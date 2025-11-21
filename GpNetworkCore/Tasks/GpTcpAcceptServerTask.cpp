#include <GpNetwork/GpNetworkCore/Tasks/GpTcpAcceptServerTask.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpTcpAcceptServerTask::GpTcpAcceptServerTask
(
    GpSocketTCP::UP         aSocketUP,
    GpSocketAddr            aListenAddr,
    size_t                  aListenMaxQueueSize,
    GpSocketFlags           aAcceptSocketFlags,
    GpIOEventPollerIdx      aIOEventPollerIdx,
    GpSocketTaskFactory::UP aSocketTaskFactoryUP
) noexcept:
GpSingleSocketTask{std::move(aSocketUP)},
iListenAddr         {aListenAddr},
iListenMaxQueueSize {aListenMaxQueueSize},
iAcceptSocketFlags  {aAcceptSocketFlags},
iIOEventPollerIdx   {aIOEventPollerIdx},
iSocketTaskFactoryUP{std::move(aSocketTaskFactoryUP)}
{
}

GpTcpAcceptServerTask::~GpTcpAcceptServerTask (void) noexcept
{
}

void    GpTcpAcceptServerTask::OnStart (void)
{
    StartListen();
}

void    GpTcpAcceptServerTask::OnStop (ExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        GpUniqueLock uniqueLock{SpinLock()};
        Socket().Close();
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& ex)
    {
        aStopExceptionsOut.emplace_back(GpException{ex.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpTcpAcceptServerTask::OnStop]: unknown exception"_sv});
    }
}

void    GpTcpAcceptServerTask::OnStopException (const GpException& aException) noexcept
{
    LOG_EXCEPTION
    (
        "[GpTcpAcceptServerTask::OnStopException]",
        aException
    );
}

void    GpTcpAcceptServerTask::OnReadyToRead (GpSocket& aSocket)
{
    if (aSocket.Id() == GpSocketId_Default()) [[unlikely]]
    {
        return;
    }

    GpSocketTCP& serverSocket = static_cast<GpSocketTCP&>(aSocket);

    // Accept
    size_t maxCount = 30;// TODO: move to config. Max accepted sockets without YIELD

    while (maxCount > 0)
    {
        maxCount--;

        GpSocketTCP::C::Opts::Val acceptedSocketOpt = serverSocket.Accept(iAcceptSocketFlags | GpSocketFlag::NO_BLOCK);

        if (!acceptedSocketOpt.has_value())
        {
            break;
        }

        GpSocketTCP&    acceptedSocket      = acceptedSocketOpt.value();
        GpSocketId      acceptedSocketId    = acceptedSocket.Id();

        // Create socket task from factory
        GpSocketTask::SP acceptedSocketTaskSP = iSocketTaskFactoryUP->NewInstance
        (
            std::make_unique<GpSocketTCP>(std::move(acceptedSocket))
        );

        // Register socket and task to IO event poller
        GpIOEventPollerCatalog::SAddSubscription
        (
            acceptedSocketId,
            acceptedSocketTaskSP,
            iIOEventPollerIdx,
            GpIOEventsTypes
            {
                GpIOEventType::READY_TO_READ,
                GpIOEventType::READY_TO_WRITE,
                GpIOEventType::CLOSED,
                GpIOEventType::ERROR_OCCURRED,
            }
        );

        // Add to scheduler
        SPAWN_READY_TASK(std::move(acceptedSocketTaskSP));
    }
}

void    GpTcpAcceptServerTask::OnReadyToWrite ([[maybe_unused]] GpSocket& aSocket)
{
    // NOP
}

void    GpTcpAcceptServerTask::OnClosed ([[maybe_unused]] GpSocket& aSocket)
{
    // NOP
}

void    GpTcpAcceptServerTask::OnError ([[maybe_unused]] GpSocket& aSocket)
{
    // NOP
}

void    GpTcpAcceptServerTask::StartListen (void)
{
    GpUniqueLock uniqueLock{SpinLock()};

    // Check if already connected
    if (VerifyBeforeListen() == true)
    {
        // Already listen iListenAddr
        return;
    }

    // New outgoing connection
    GpSocketTCP& socketTcp = static_cast<GpSocketTCP&>(Socket());

    socketTcp.Listen(iListenAddr, iListenMaxQueueSize);
}

bool    GpTcpAcceptServerTask::VerifyBeforeListen (void) const
{
    const GpSocketTCP&              socketTcp   = static_cast<const GpSocketTCP&>(Socket());
    const GpSocketStateTCP::EnumT   socketState = socketTcp.State();

    switch (socketState)
    {
        case GpSocketStateTCP::NOT_CONNECTED:
        {
            // OK
            return false;
        } break;
        case GpSocketStateTCP::LISTEN:
        {
            // Check to what ip connected
            VERIFY
            (
                socketTcp.AddrLocal() == iListenAddr,
                [&]()
                {
                    return fmt::format
                    (
                        "Failed to listen '{}': socket is in listening mode on '{}'",
                        iListenAddr.ToString(),
                        socketTcp.AddrLocal().ToString()
                    );
                }
            );
        } break;
        case GpSocketStateTCP::CONNECTION_IN_PROGRESS:
        {
            THROW(fmt::format(
                "Failed to listen '{}': another connection is in progress to '{}'",
                iListenAddr.ToString(),
                socketTcp.AddrRemote().ToString()
            ));
        } break;
        case GpSocketStateTCP::OUTGOING:
        {
            THROW(fmt::format(
                "Failed to listen '{}': already connected to '{}'",
                iListenAddr.ToString(),
                socketTcp.AddrRemote().ToString()
            ));
        } break;
        case GpSocketStateTCP::INCOMING:
        {
            THROW(fmt::format(
                "Failed to listen '{}': socket is in incoming connection mode from '{}'",
                iListenAddr.ToString(),
                socketTcp.AddrRemote().ToString()
            ));
        } break;
    };

    return true;
}

}// namespace GPlatform

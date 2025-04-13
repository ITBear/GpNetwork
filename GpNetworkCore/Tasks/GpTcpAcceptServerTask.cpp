#include <GpNetwork/GpNetworkCore/Tasks/GpTcpAcceptServerTask.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpTcpAcceptServerTask::GpTcpAcceptServerTask
(
    GpSocketAddr                aListenAddr,
    const GpSocketFlags         aListenSocketFlags,
    const size_t                aListenMaxQueueSize,
    const GpSocketFlags         aAcceptSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    GpTcpServerTaskFactory::SP  aServerTaskFactory
) noexcept:
iListenAddr        {aListenAddr},
iListenSocketFlags {aListenSocketFlags},
iListenMaxQueueSize{aListenMaxQueueSize},
iAcceptSocketFlags {aAcceptSocketFlags},
iIOEventPollerIdx  {aIOEventPollerIdx},
iServerTaskFactory {std::move(aServerTaskFactory)}
{
}

GpTcpAcceptServerTask::~GpTcpAcceptServerTask (void) noexcept
{
}

void    GpTcpAcceptServerTask::OnStart (void)
{
    GpSocketsTask::OnStart();

    // Configure listen socket
    {
        // Create listen socket
        iListenSocketSP = MakeSP<GpSocketTCP>
        (
            iListenSocketFlags | GpSocketFlag::NO_BLOCK,
            GpSocket::CloseModeT::CLOSE_ON_DESTRUCT
        );

        // Listen socket
        iListenSocketSP.Vn().Listen(iListenAddr, iListenMaxQueueSize);

        // Add socket to io poller subscription
        const bool isAdded = GpIOEventPollerCatalog::SAddSubscriptionSafe
        (
            iListenSocketSP.Vn().Id(),
            TaskId(),
            iIOEventPollerIdx,
            {GpIOEventType::READY_TO_READ, GpIOEventType::ERROR_OCCURRED}
        );

        VERIFY
        (
            isAdded == true,
            "Failed to add subscription to IO event poller"
        );
    }
}

void    GpTcpAcceptServerTask::OnStop (ExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        if (iListenSocketSP.IsNotNULL())
        {
            const GpSocketId socketId = iListenSocketSP.Vn().Id();

            if (socketId != GpSocketId_Default()) [[likely]]
            {
                // Check if IO event poller still exists
                GpIOEventPoller::C::Opts::SP eventPollerOpt = GpIOEventPollerCatalog::S().GetByIdxOpt(iIOEventPollerIdx);

                if (eventPollerOpt.has_value())
                {
                    // Remove socket from IO poller subscription
                    const bool isRemoved = GpIOEventPollerCatalog::SRemoveSubscriptionSafe
                    (
                        socketId,
                        TaskId(),
                        iIOEventPollerIdx
                    );

                    VERIFY
                    (
                        isRemoved == true,
                        fmt::format
                        (
                            "Failed to remove subscription from IO event poller (idx {}). Socket id {}",
                            iIOEventPollerIdx.Value(),
                            socketId
                        )
                    );
                }

                iListenSocketSP.Vn().Close();
                OnClosed(iListenSocketSP.Vn());
            }
        }
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& e)
    {
        aStopExceptionsOut.emplace_back(GpException{e.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpTcpAcceptServerTask::OnStop]: unknown exception"_sv});
    }

    GpSocketsTask::OnStop(aStopExceptionsOut);
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
        std::ignore = RequestStop();
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

        GpSocketTCP& acceptedSocket = acceptedSocketOpt.value();

        // Create socket task from factory
        GpTcpServerTask::SP serverTaskSP = iServerTaskFactory.Vn().NewInstance
        (
            MakeSP<GpSocketTCP>(std::move(acceptedSocket)),
            iIOEventPollerIdx
        );

        // Add to scheduler
        if (GpTaskScheduler::S().NewToReady(std::move(serverTaskSP)) == false)
        {
            break;
        }
    }
}

void    GpTcpAcceptServerTask::OnReadyToWrite (GpSocket& aSocket)
{
    if (aSocket.Id() == GpSocketId_Default()) [[unlikely]]
    {
        std::ignore = RequestStop();
        return;
    }

    LOG_INFO
    (
        fmt::format
        (
            "[GpTcpAcceptServerTask::OnReadyToWrite]: socket id {}",
            aSocket.Id()
        )
    );
}

void    GpTcpAcceptServerTask::OnClosed (GpSocket& aSocket)
{
    if (aSocket.Id() == GpSocketId_Default()) [[unlikely]]
    {
        std::ignore = RequestStop();
        return;
    }

    LOG_INFO
    (
        fmt::format
        (
            "[GpTcpAcceptServerTask::OnClosed]: socket id {}",
            aSocket.Id()
        )
    );

    std::ignore = RequestStop();
}

void    GpTcpAcceptServerTask::OnError (GpSocket& aSocket)
{
    if (aSocket.Id() == GpSocketId_Default()) [[unlikely]]
    {
        std::ignore = RequestStop();
        return;
    }

    LOG_INFO
    (
        fmt::format
        (
            "[GpTcpAcceptServerTask::OnError]: socket id {}",
            aSocket.Id()
        )
    );

    std::ignore = RequestStop();
}

void    GpTcpAcceptServerTask::ProcessOtherMessages ([[maybe_unused]] GpAny& aMessage)
{
    // TODO: add message to log
}

GpSocket::SP    GpTcpAcceptServerTask::FindSocket ([[maybe_unused]] GpSocketId aSocketId)
{
    return iListenSocketSP;
}

}// namespace GPlatform

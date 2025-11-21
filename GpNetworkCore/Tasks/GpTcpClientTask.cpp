#include <GpNetwork/GpNetworkCore/Tasks/GpTcpClientTask.hpp>

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpNetwork/GpNetworkCore/GpNetworkUtilsDns.hpp>

namespace GPlatform {

GpTcpClientTask::GpTcpClientTask (GpSocketTCP::UP aSocketUP) noexcept:
GpSingleSocketTask{std::move(aSocketUP)}
{
}

GpTcpClientTask::GpTcpClientTask
(
    GpSocketTCP::UP aSocketUP,
    std::string     aTaskName
) noexcept:
GpSingleSocketTask{std::move(aSocketUP), std::move(aTaskName)}
{
}

GpTcpClientTask::~GpTcpClientTask (void) noexcept
{
}

GpSocketStateTCP    GpTcpClientTask::ConnectTo
(
    std::string_view        aDomainName,
    const u_int_16          aPort,
    const GpSocketIPv       aIPv,
    const milliseconds_t    aConnectTimeout,
    GpTask::WP              aTaskWP
)
{
    // Get server ip from domain name
    GpSocketAddr serverAddr;
    {
        GpSocketAddr connectedToAddr;
        {
            GpUniqueLock uniqueLock{SpinLock()};
            connectedToAddr = Socket().AddrRemote();
        }

        //serverAddr = GpNetworkUtilsDns::S().Resolve(aDomainName, aIPv, connectedToAddr);
        serverAddr = GpNetworkUtilsDns::SResolveNoCache(aDomainName, aIPv).addresses.at(0);
        serverAddr.SetPort(aPort);
    }

    // Connect
    return ConnectTo(serverAddr, aConnectTimeout, std::move(aTaskWP));
}

GpSocketStateTCP    GpTcpClientTask::ConnectTo
(
    const GpSocketAddr&     aServerAddr,
    const milliseconds_t    aConnectTimeout,
    GpTask::WP              aTaskWP
)
{
    GpUniqueLock uniqueLock{SpinLock()};

    // Check if already connected
    if (VerifyConnectTo(aServerAddr) == true)
    {
        // Already connected to aServerAddr
        OnConnected(static_cast<GpSocketTCP&>(Socket()));
        return GpSocketStateTCP::OUTGOING;
    }

    // New outgoing connection
    GpSocketTCP&            socketTcp   = static_cast<GpSocketTCP&>(Socket());
    GpSocketStateTCP::EnumT socketState = socketTcp.Connect(aServerAddr);

    if (socketState == GpSocketStateTCP::OUTGOING)
    {
        // Connected to aServerAddr
        return GpSocketStateTCP::OUTGOING;
    }

    // Setup timeout
    {
        GpTimersManager::SSingleShot
        (
            [aTaskWP](const GpTimer&)
            {
                if (auto taskSP = aTaskWP.Lock(); taskSP.IsNotNULL())
                {
                    GpTcpClientTask& task = static_cast<GpTcpClientTask&>(taskSP.Vn());
                    task.PushSocketEvents(-1, {GpIOEventType::CONNECTION_TIMEOUT});
                    WAKEUP_TASK(task);
                }
            },
            aConnectTimeout
        );
    }

    return GpSocketStateTCP::CONNECTION_IN_PROGRESS;
}

void    GpTcpClientTask::OnStop (ExceptionsT& aStopExceptionsOut) noexcept
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
        aStopExceptionsOut.emplace_back(GpException{"[GpTcpClientTask::OnStop]: unknown exception"_sv});
    }
}

void    GpTcpClientTask::ProcessSocketEvents
(
    GpSocket&       aSocket,
    GpIOEventsTypes aIoEvents
)
{
    GpSocketTCP& socketTcp = static_cast<GpSocketTCP&>(aSocket);

    if (aIoEvents.Test(GpIOEventType::READY_TO_READ))
    {
        OnReadyToRead(socketTcp);
    }

    if (aIoEvents.Test(GpIOEventType::READY_TO_WRITE))
    {
        OnReadyToWrite(socketTcp);

        if (!iIsConnected) [[unlikely]]
        {
            socketTcp.OnConnected();
            iIsConnected = true;
            OnConnected(socketTcp);
        }
    }

    if (aIoEvents.Test(GpIOEventType::CLOSED)) [[unlikely]]
    {
        OnClosed(socketTcp);
    } else if (aIoEvents.Test(GpIOEventType::ERROR_OCCURRED)) [[unlikely]]
    {
        OnError(socketTcp);
    } else if (aIoEvents.Test(GpIOEventType::CONNECTION_TIMEOUT)) [[unlikely]]
    {
        if (!iIsConnected)
        {
            socketTcp.OnConnectionFailed();
            OnConnectionTimeout(socketTcp);
        }
    }
}

bool    GpTcpClientTask::VerifyConnectTo (const GpSocketAddr& aServerAddr) const
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
            THROW(fmt::format(
                "Failed to connect to '{}': socket is in listening mode on '{}'",
                aServerAddr.ToString(),
                socketTcp.AddrLocal().ToString()
            ));
        } break;
        case GpSocketStateTCP::CONNECTION_IN_PROGRESS:
        {
            THROW(fmt::format(
                "Failed to connect to '{}': another connection is in progress to '{}'",
                aServerAddr.ToString(),
                socketTcp.AddrRemote().ToString()
            ));
        } break;
        case GpSocketStateTCP::OUTGOING:
        {
            // Check to what ip connected
            VERIFY
            (
                socketTcp.AddrRemote() == aServerAddr,
                [&]()
                {
                    return fmt::format
                    (
                        "Failed to connect to '{}': already connected to '{}'",
                        aServerAddr.ToString(),
                        socketTcp.AddrRemote().ToString()
                    );
                }
            );
        } break;
        case GpSocketStateTCP::INCOMING:
        {
            THROW(fmt::format(
                "Failed to connect to '{}': socket is in incoming connection mode from '{}'",
                aServerAddr.ToString(),
                socketTcp.AddrRemote().ToString()
            ));
        } break;
    };

    return true;
}

}// namespace GPlatform

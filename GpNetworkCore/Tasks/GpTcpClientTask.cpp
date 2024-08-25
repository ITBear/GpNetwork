#include <GpNetwork/GpNetworkCore/Tasks/GpTcpClientTask.hpp>

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpNetwork/GpNetworkCore/GpNetworkUtilsDns.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpTcpClientConnectionState)

GpTcpClientTask::GpTcpClientTask
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx
):
iSocketFlags     {aSocketFlags},
iIOEventPollerIdx{aIOEventPollerIdx}
{
}

GpTcpClientTask::GpTcpClientTask
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    std::string                 aTaskName
):
GpSocketsTask{std::move(aTaskName)},
iSocketFlags     {aSocketFlags},
iIOEventPollerIdx{aIOEventPollerIdx}
{
}

GpTcpClientTask::~GpTcpClientTask (void) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

    _CloseConnection();
}

void    GpTcpClientTask::CloseTcpConnection (void)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

    _CloseConnection();
}

void    GpTcpClientTask::ConnectTcpAndWait
(
    std::string_view        aDomainName,
    const u_int_16          aPort,
    const GpSocketIPv       aIPv,
    const milliseconds_t    aConnectTimeout
)
{
    // Get server ip from domain name
    GpSocketAddr serverAddr;
    {
        GpSocketAddr connectedToAddr;
        {
            GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};
            connectedToAddr = iConnectedToAddr;
        }

        serverAddr = GpNetworkUtilsDns::S().Resolve(aDomainName, aIPv, connectedToAddr);
        serverAddr.SetPort(aPort);
    }

    // Connect
    ConnectTcpAndWait(serverAddr, aConnectTimeout);
}

void    GpTcpClientTask::ConnectTcpAndWait
(
    const GpSocketAddr&     aServerAddr,
    const milliseconds_t    aConnectTimeout
)
{
    // Check and start connection
    GpSocketTCP::SP     localSocketTcpSP;
    GpIOEventPollerIdx  localIOEventPollerIdx;
    GpTaskId            localIOEventPollerSubscribeTaskId;

    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

        CheckBeforeNewTcpConnection(aServerAddr);

        // Check if connected
        if (iConnectionState == ConnectionStateT::CONNECTED)
        {
            return;
        }

        // Create socket
        iSocketTCP = MakeSP<GpSocketTCP>
        (
            iSocketFlags | GpSocketFlag::NO_BLOCK,
            GpSocket::CloseModeT::CLOSE_ON_DESTRUCT
        );

        GpSocketTCP& socketTCP = iSocketTCP.Vn();

        // Init socket
        socketTCP.Create(aServerAddr.IPv());

        iIOEventPollerSubscribeTaskId       = GpTask::SCurrentTask().value().get().TaskId();
        iConnectionState                    = ConnectionStateT::CONNECTION_IN_PROGRESS;
        iConnectedToAddr                    = aServerAddr;

        localSocketTcpSP                    = iSocketTCP;
        localIOEventPollerIdx               = iIOEventPollerIdx;
        localIOEventPollerSubscribeTaskId   = iIOEventPollerSubscribeTaskId;
    }

    // Wait for connection  
    localSocketTcpSP->ConnectAndWait(aServerAddr, aConnectTimeout, localIOEventPollerIdx, localIOEventPollerSubscribeTaskId);

    // Pop event
    GpAny::C::Opt::Val msg = PopMessage();

    //
    OnConnected(localSocketTcpSP.Vn());
}

void    GpTcpClientTask::OnStart (void)
{
    GpSocketsTask::OnStart();
}

void    GpTcpClientTask::OnStop (StopExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

        _CloseConnection();
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

    GpSocketsTask::OnStop(aStopExceptionsOut);
}

void    GpTcpClientTask::ProcessOtherMessages (GpAny& aMessage)
{
    THROW_GP
    (
        fmt::format
        (
            "Get not socket message {}",
            aMessage.TypeInfo().name()
        )
    );
}

GpSocket::SP    GpTcpClientTask::FindSocket ([[maybe_unused]] GpSocketId aSocketId)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

    return iSocketTCP;
}

void    GpTcpClientTask::CheckBeforeNewTcpConnection (const GpSocketAddr& aServerAddr)
{
    if (iConnectionState == ConnectionStateT::NOT_CONNECTED)
    {
        return;
    }

    THROW_COND_GP
    (
        iConnectionState == ConnectionStateT::CONNECTED,
        "Other connection in progress"
    );

    // Check to what ip connected
    if (iConnectedToAddr != aServerAddr)
    {
        _CloseConnection();
        iConnectedToAddr = aServerAddr;
    } else
    {
        // OK, connected to correct ip
        return;
    }
}

void    GpTcpClientTask::_CloseConnection (void)
{
    if (iSocketTCP.IsNotNULL())
    {
        std::ignore = GpIOEventPollerCatalog::SRemoveSubscriptionSafe
        (
            iSocketTCP.Vn().Id(),
            iIOEventPollerSubscribeTaskId,
            iIOEventPollerIdx
        );

        iSocketTCP.Vn().Close();
        iSocketTCP.Clear();
    }

    iConnectionState = ConnectionStateT::NOT_CONNECTED;
    iConnectedToAddr.Clear();   
}

}// namespace GPlatform

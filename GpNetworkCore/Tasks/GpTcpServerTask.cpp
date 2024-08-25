#include <GpNetwork/GpNetworkCore/Tasks/GpTcpServerTask.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>

namespace GPlatform {

GpTcpServerTask::GpTcpServerTask
(
    GpSocketTCP::SP     aSocketTCP,
    GpIOEventPollerIdx  aIOEventPollerIdx
):
iIOEventPollerIdx{aIOEventPollerIdx},
iSocketTCP       {std::move(aSocketTCP)}
{
}

GpTcpServerTask::GpTcpServerTask
(
    GpSocketTCP::SP     aSocketTCP,
    GpIOEventPollerIdx  aIOEventPollerIdx,
    std::string         aTaskName
):
GpSocketsTask{std::move(aTaskName)},
iIOEventPollerIdx{aIOEventPollerIdx},
iSocketTCP       {std::move(aSocketTCP)}
{
}

GpTcpServerTask::~GpTcpServerTask (void) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

    _CloseConnection();
}

void    GpTcpServerTask::OnStart (void)
{
    GpSocketsTask::OnStart();

    // Subscribe to IO event poller
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

        iIOEventPollerSubscribeTaskId   = GpTask::SCurrentTask().value().get().TaskId();
        const GpSocketId    socketId    = iSocketTCP->Id();
        const bool          isAdded     = GpIOEventPollerCatalog::SAddSubscriptionSafe
        (
            socketId,
            iIOEventPollerSubscribeTaskId,
            iIOEventPollerIdx,
            {GpIOEventType::READY_TO_READ, GpIOEventType::READY_TO_WRITE, GpIOEventType::CLOSED, GpIOEventType::ERROR_OCCURRED}
        );

        THROW_COND_GP
        (
            isAdded == true,
            "Failed to subscribe to IO event poller"
        );
    }
}

void    GpTcpServerTask::OnStop (StopExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

        _CloseConnection();
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& e)
    {
        aStopExceptionsOut.emplace_back(GpException{e.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpTcpServerTask::OnStop]: unknown exception"_sv});
    }

    GpSocketsTask::OnStop(aStopExceptionsOut);
}

void    GpTcpServerTask::ProcessOtherMessages (GpAny& aMessage)
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

GpSocket::SP    GpTcpServerTask::FindSocket ([[maybe_unused]] GpSocketId aSocketId)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};

    return iSocketTCP;
}

void    GpTcpServerTask::_CloseConnection (void)
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
}

}// namespace GPlatform

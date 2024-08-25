#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Server/GpHttpServer.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Server/GpHttpServerRequestTaskFactory.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedFutureUtils.hpp>

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpHttpServer::GpHttpServer (void) noexcept
{
}

GpHttpServer::GpHttpServer
(
    GpHttpServerCfgDesc aServerCfgDesc,
    GpHttpRouter::SP    aRouter
):
iServerCfgDesc{std::move(aServerCfgDesc)},
iRouter       {std::move(aRouter)}
{
}

GpHttpServer::~GpHttpServer (void) noexcept
{
}

void    GpHttpServer::Start (void)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    THROW_COND_GP
    (
        iAcceptSocketTask.IsNULL(),
        "Already started"_sv
    );

    // Create accept sockets task
    GpSocketAddr listenAddr;
    listenAddr.SetAutoIPv(iServerCfgDesc.listen_ip, iServerCfgDesc.listen_port);

    const GpIOEventPollerIdx ioEventPollerIdx = GpIOEventPollerCatalog::S().IdxByName(iServerCfgDesc.event_poller_name);

    iAcceptSocketTask = MakeSP<GpTcpAcceptServerTask>
    (
        listenAddr,
        iServerCfgDesc.listen_socket_flags,
        iServerCfgDesc.listen_max_queue_size,       
        iServerCfgDesc.accept_socket_flags,
        ioEventPollerIdx,
        MakeSP<GpHttpServerRequestTaskFactory>(iRouter)
    );

    // Add to scheduler and start
    GpTaskScheduler::S().NewToReady(iAcceptSocketTask);

    // Wait for start
    GpTask::StartFutureT::SP startFuture = iAcceptSocketTask->GetStartFuture();

    GpItcSharedFutureUtils::SWaitForInf
    (
        startFuture.V(),
        [&](typename GpTaskFiber::StartFutureT::value_type&)// OnSuccessFnT
        {
            LOG_INFO("[GpHttpServer::Start]: started"_sv);
        },
        [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        },
        100.0_si_ms
    );
}

void    GpHttpServer::Start
(
    GpHttpServerCfgDesc aServerCfgDesc,
    GpHttpRouter::SP    aRouter
)
{
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

        THROW_COND_GP
        (
            iAcceptSocketTask.IsNULL(),
            "Already started"_sv
        );

        iServerCfgDesc  = std::move(aServerCfgDesc);
        iRouter         = std::move(aRouter);
    }   

    Start();
}

void    GpHttpServer::RequestAndWaitForStop (void)
{
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

        if (iAcceptSocketTask.IsNULL())
        {
            return;
        }
    }

    // Request stop
    GpTask::DoneFutureT::SP acceptSocketTaskDoneFuture;
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
        acceptSocketTaskDoneFuture = iAcceptSocketTask.Vn().RequestTaskStop();
    }

    // Wait for stop
    GpItcSharedFutureUtils::SWaitForInf
    (
        acceptSocketTaskDoneFuture.V(),
        [&](typename GpTaskFiber::DoneFutureT::value_type&)// OnSuccessFnT
        {
            LOG_INFO("[GpHttpServer::RequestAndWaitForStop]: done"_sv);
        },
        [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        },
        100.0_si_ms
    );
}

}// namespace GPlatform

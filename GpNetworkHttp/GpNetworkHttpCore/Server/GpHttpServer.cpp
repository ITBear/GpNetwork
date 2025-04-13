#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Server/GpHttpServer.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Server/GpHttpServerRequestTaskFactory.hpp>
#include <GpCore2/GpTasks/ITC/GpItcFutureUtils.hpp>

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

    VERIFY
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
    std::ignore = GpTaskScheduler::S().NewToReady(iAcceptSocketTask);

    // Wait for start
    GpTask::StartFutureT::SP startFuture = iAcceptSocketTask->StartFuture();

    GpItcFutureUtils::SWait
    (
        startFuture.V(),
        [&](typename GpTaskFiber::StartFutureT::value_type&)// OnSuccessFnT
        {
            LOG_INFO("[GpHttpServer::Start]: started"_sv);
        },
        [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        }
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

        VERIFY
        (
            iAcceptSocketTask.IsNULL(),
            "Already started"_sv
        );

        iServerCfgDesc  = std::move(aServerCfgDesc);
        iRouter         = std::move(aRouter);
    }   

    Start();
}

void    GpHttpServer::RequestStopAndWait (void)
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

        auto acceptSocketTaskDoneFutureOpt = iAcceptSocketTask.Vn().RequestStop();

        VERIFY
        (
            acceptSocketTaskDoneFutureOpt.has_value(),
            "Failed to stop HTTP server task"
        );

        acceptSocketTaskDoneFuture = acceptSocketTaskDoneFutureOpt.value();
    }

    // Wait for stop
    GpItcFutureUtils::SWait
    (
        acceptSocketTaskDoneFuture.V(),
        [&](typename GpTaskFiber::DoneFutureT::value_type&)// OnSuccessFnT
        {
            LOG_INFO("[GpHttpServer::RequestStopAndWait]: done"_sv);
        },
        [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        }
    );
}

}// namespace GPlatform

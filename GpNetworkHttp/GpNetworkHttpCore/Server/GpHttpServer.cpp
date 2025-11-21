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
    GpUniqueLock uniqueLock{iSpinLock};

    VERIFY
    (
        iAcceptSocketTask.IsNULL(),
        "Already started"_sv
    );

    // Create accept sockets task
    GpSocketAddr listenAddr;
    listenAddr.SetAutoIPv(iServerCfgDesc.listen_ip, iServerCfgDesc.listen_port);

    const GpIOEventPollerIdx ioEventPollerIdx = GpIOEventPollerCatalog::S().IdxByName(iServerCfgDesc.event_poller_name);

    GpSocketTCP::UP socketTcpUP = std::make_unique<GpSocketTCP>
    (
        iServerCfgDesc.listen_socket_flags | GpSocketFlag::NO_BLOCK,
        GpSocket::CloseModeT::CLOSE_ON_DESTRUCT
    );

    iAcceptSocketTask = MakeSP<GpTcpAcceptServerTask>
    (
        std::move(socketTcpUP),
        listenAddr,
        iServerCfgDesc.listen_max_queue_size,
        iServerCfgDesc.accept_socket_flags,
        ioEventPollerIdx,
        std::make_unique<GpHttpServerRequestTaskFactory>(iRouter)
    );

    // Add to scheduler and start
    SPAWN_READY_TASK(iAcceptSocketTask);

    // Wait for start
    GpTask::StartFutureT::SP startFuture = iAcceptSocketTask->StartFuture();

    GpItcFutureUtils::SWait
    (
        startFuture.V(),
        [&](typename GpTaskFiber::StartFutureT::value_type&&)// OnSuccessFnT
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
        GpUniqueLock uniqueLock{iSpinLock};

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
        GpUniqueLock uniqueLock{iSpinLock};

        if (iAcceptSocketTask.IsNULL())
        {
            return;
        }
    }

    // Request stop
    GpTask::DoneFutureT::SP acceptSocketTaskDoneFuture;
    {
        GpUniqueLock uniqueLock{iSpinLock};
        acceptSocketTaskDoneFuture = iAcceptSocketTask.Vn().RequestStop();
    }

    // Wait for stop
    GpItcFutureUtils::SWait
    (
        acceptSocketTaskDoneFuture.V(),
        [&](typename GpTaskFiber::DoneFutureT::value_type&&)// OnSuccessFnT
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

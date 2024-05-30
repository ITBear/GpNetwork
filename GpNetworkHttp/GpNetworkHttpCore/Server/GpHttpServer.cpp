#include "GpHttpServer.hpp"
#include "GpHttpRequestSocketTaskFactory.hpp"

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

#include <iostream>

namespace GPlatform {

GpHttpServer::GpHttpServer (void) noexcept
{
}

GpHttpServer::GpHttpServer
(
    GpHttpServerCfgDesc aServerCfgDesc,
    GpHttpRouter::SP    aRouter
):
iServerCfgDesc(std::move(aServerCfgDesc)),
iRouter       (std::move(aRouter))
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
    GpSocketAddr sockAddr;
    sockAddr.SetAutoIPv(iServerCfgDesc.listen_ip, iServerCfgDesc.listen_port);

    const GpIOEventPollerIdx ioEventPollerIdx = GpIOEventPollerCatalog::S().IdxByName(iServerCfgDesc.event_poller_name);

    iAcceptSocketTask = MakeSP<GpTcpAcceptServerTask>
    (
        sockAddr,
        iServerCfgDesc.listen_max_queue_size,
        iServerCfgDesc.listen_socket_flags,
        iServerCfgDesc.accept_socket_flags,
        ioEventPollerIdx,
        MakeSP<GpHttpRequestSocketTaskFactory>(iRouter)
    );

    // Add to scheduler and start
    GpTaskScheduler::S().NewToReady(iAcceptSocketTask);

    // Wait for start
    GpTask::StartFutureT::SP startFuture = iAcceptSocketTask->GetStartFuture();

    while (!startFuture.Vn().WaitFor(100.0_si_ms))
    {
        // NOP
    }

    // Check start result
    GpTask::StartFutureT::SCheckIfReady
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
        acceptSocketTaskDoneFuture = iAcceptSocketTask.Vn().RequestStop();
    }

    // Wait for stop
    while (!acceptSocketTaskDoneFuture.Vn().WaitFor(100.0_si_ms))
    {
        // NOP
    }

    // Check stop result
    GpTask::DoneFutureT::SCheckIfReady
    (
        acceptSocketTaskDoneFuture.V(),
        [&](typename GpTaskFiber::DoneFutureT::value_type&)// OnSuccessFnT
        {
            LOG_INFO("[GpHttpServer::RequestAndWaitForStop]: done"_sv);
        },
        [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        }
    );
}

}// namespace GPlatform

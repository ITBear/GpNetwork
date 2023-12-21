#include "GpHttpServer.hpp"
#include "GpHttpRequestSocketTaskFactory.hpp"

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>

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
    std::scoped_lock lock(iMutex);

    THROW_COND_GP
    (
        iAcceptSocketsTask.IsNULL(),
        u8"Already started"_sv
    );

    // Create accept sockets task
    GpSocketAddr sockAddr;
    sockAddr.SetAutoIPv(iServerCfgDesc.listen_ip, iServerCfgDesc.listen_port);

    iAcceptSocketsTask = MakeSP<GpTcpAcceptServerTask>
    (
        sockAddr,
        iServerCfgDesc.listen_max_queue_size,
        iServerCfgDesc.listen_socket_flags,
        iServerCfgDesc.accept_socket_flags,
        iServerCfgDesc.event_poller_name,
        MakeSP<GpHttpRequestSocketTaskFactory>()
    );

    // Add to scheduler
    GpTaskScheduler::S().NewToReady(std::move(iAcceptSocketsTask));

    // TODO: add task dependency (this to iAcceptSocketsTask)
}

void    GpHttpServer::Start
(
    GpHttpServerCfgDesc aServerCfgDesc,
    GpHttpRouter::SP    aRouter
)
{
    {
        std::scoped_lock lock(iMutex);

        THROW_COND_GP
        (
            iAcceptSocketsTask.IsNULL(),
            u8"Already started"_sv
        );

        iServerCfgDesc  = std::move(aServerCfgDesc);
        iRouter         = std::move(aRouter);
    }

    Start();
}

void    GpHttpServer::RequestStop (void)
{

}

void    GpHttpServer::WaitForStop (void)
{

}

}//namespace GPlatform

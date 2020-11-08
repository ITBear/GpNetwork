#include "GpHttpServerNode.hpp"
#include "GpHttpServerNodeSocketTaskFactory.hpp"
#include "../../IO/Sockets/GpTcpServerTask.hpp"

namespace GPlatform {

GpHttpServerNode::GpHttpServerNode (const GpSocketAddr&             aListenSocketAddr,
                                    GpIOEventPollerFactory::SP      aEventPollerFactory,
                                    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
GpHttpServer(std::move(aRequestHandlerFactory)),
iListenSocketAddr(aListenSocketAddr),
iEventPollerFactory(std::move(aEventPollerFactory))
{
}

GpHttpServerNode::~GpHttpServerNode (void) noexcept
{
}

void    GpHttpServerNode::OnStart (void)
{
    StartIOPoller();
    StartTcpServer();
}

void    GpHttpServerNode::OnStep (EventOptRefT /*aEvent*/)
{
    //NOP
}

void    GpHttpServerNode::OnStop (void) noexcept
{
}

void    GpHttpServerNode::StartIOPoller (void)
{
    GpTaskFiberBarrier::SP ioPollerStartBarrier = MakeSP<GpTaskFiberBarrier>();

    iIOPoller = iEventPollerFactory->NewInstance(GpTaskFiberBarrierLock(ioPollerStartBarrier));
    GpTaskScheduler::SCurrentScheduler().value()->AddTaskToReady(iIOPoller);

    GpTaskFiberBarrierWaiter ioPollerStartWaiter(ioPollerStartBarrier);
    ioPollerStartWaiter.Wait(GetWeakPtr());
}

void    GpHttpServerNode::StartTcpServer (void)
{
    GpTaskScheduler::WP taskScheduler = GpTaskScheduler::SCurrentScheduler().value();

    //TODO: move to config
    GpTask::SP tcpServerTask = GpTcpServerTask::SConstruct(iListenSocketAddr,
                                                           GpSocketFlags{GpSocketFlag::REUSE_ADDR, GpSocketFlag::REUSE_PORT},   //TODO: move to config
                                                           256_cnt, //TODO: move to config
                                                           iIOPoller,
                                                           MakeSP<GpHttpServerNodeSocketTaskFactory>(),
                                                           taskScheduler);

    //TODO: add waiting for iIOPoller OnStartDone()
    taskScheduler->AddTaskToReady(tcpServerTask);
}

}//namespace GPlatform
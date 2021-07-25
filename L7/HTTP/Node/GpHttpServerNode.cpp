#include "GpHttpServerNode.hpp"
#include "GpHttpServerNodeSocketTaskFactory.hpp"
#include "../../../IO/Sockets/GpTcpServerTask.hpp"

#include <iostream>

namespace GPlatform {

GpHttpServerNode::GpHttpServerNode
(
    std::string_view                aName,
    const GpSocketAddr&             aListenSocketAddr,
    GpIOEventPoller::WP             aEventPoller,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
):
GpHttpServer(aName, std::move(aRequestHandlerFactory)),
iListenSocketAddr(aListenSocketAddr),
iEventPoller(std::move(aEventPoller))
{
}

GpHttpServerNode::~GpHttpServerNode (void) noexcept
{
}

void    GpHttpServerNode::OnStart (void)
{
    StartTcpServer(Name() + ": TCP server task"_sv);
}

GpTask::ResT    GpHttpServerNode::OnStep (EventOptRefT /*aEvent*/)
{
    return GpTask::ResT::WAITING;
}

void    GpHttpServerNode::OnStop (void) noexcept
{
    iEventPoller.Clear();
}

void    GpHttpServerNode::StartTcpServer (std::string_view aTcpServerTaskName)
{
    GpTaskScheduler::WP taskScheduler = GpTaskScheduler::SCurrentScheduler().value();

    //TODO: move to config
    GpTask::SP tcpServerTask = GpTcpServerTask::SConstruct
    (
        aTcpServerTaskName,
        iListenSocketAddr,
        GpSocketFlags{GpSocketFlag::REUSE_ADDR,
                      GpSocketFlag::REUSE_PORT,
                      GpSocketFlag::NO_DELAY},  //TODO: move to config
        256_cnt,    //TODO: move to config
        iEventPoller,
        MakeSP<GpHttpServerNodeSocketTaskFactory>(RequestHandlerFactory()),
        taskScheduler
    );

    //TODO: add waiting for iIOPoller OnStartDone()
    taskScheduler->AddTaskToReady(tcpServerTask);
}

}//namespace GPlatform

#include "GpHttpServerBasic.hpp"
#include "GpHttpServerBasicSocketTaskFactory.hpp"

namespace GPlatform {

GpHttpServerBasic::~GpHttpServerBasic (void) noexcept
{
}

void    GpHttpServerBasic::OnStart (void)
{
    StartTcpServer(Name() + ": HTTP basic server task"_sv);
}

GpTaskDoRes GpHttpServerBasic::OnStep (EventOptRefT /*aEvent*/)
{
    return GpTaskDoRes::WAITING;
}

void    GpHttpServerBasic::OnStop (void) noexcept
{
}

void    GpHttpServerBasic::StartTcpServer (std::string aTcpServerTaskName)
{
    //TODO: move to config
    GpTask::SP tcpServerTask = GpTcpServerTask::SConstruct
    (
        std::move(aTcpServerTaskName),
        iListenSocketAddr,
        GpSocketFlags
        {
            GpSocketFlag::REUSE_ADDR,
            GpSocketFlag::REUSE_PORT,
            GpSocketFlag::NO_DELAY
        },      //TODO: move to config
        256,    //TODO: move to config
        iEventPoller,
        MakeSP<GpHttpServerBasicSocketTaskFactory>(RequestHandlerFactory())
    );

    std::ignore = GpTaskScheduler::S().NewToReadyDepend(tcpServerTask);
}

}//namespace GPlatform

#pragma once

#include "GpSocketTask.hpp"
#include "GpSocketTCP.hpp"
#include "GpSocketTaskFactory.hpp"

namespace GPlatform {

class GPNETWORK_API GpTcpServerTask final: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTcpServerTask)
    CLASS_DECLARE_DEFAULTS(GpTcpServerTask)

public:
                                GpTcpServerTask     (std::string_view           aName,
                                                     GpIOEventPoller::WP        aIOPoller,
                                                     GpSocketTaskFactory::SP    aTaskFactory,
                                                     GpTaskScheduler::WP        aTaskScheduler,
                                                     GpSocketTCP::SP            aSocket);
    virtual                     ~GpTcpServerTask    (void) noexcept override final;

    static  GpTcpServerTask::SP SConstruct          (std::string_view           aServerTaskName,
                                                     const GpSocketAddr&        aAddr,
                                                     const GpSocketFlags&       aFlags,
                                                     const count_t              aMaxListenQueueSize,
                                                     GpIOEventPoller::WP        aIOPoller,
                                                     GpSocketTaskFactory::SP    aTaskFactory,
                                                     GpTaskScheduler::WP        aTaskScheduler);

protected:
    virtual GpTask::ResT        OnSockReadyToRead   (GpSocket& aSocket) override final;
    virtual GpTask::ResT        OnSockReadyToWrite  (GpSocket& aSocket) override final;
    virtual void                OnSockClosed        (GpSocket& aSocket) override final;
    virtual void                OnSockError         (GpSocket& aSocket) override final;

private:
    GpSocketTaskFactory::SP     iTaskFactory;
    GpTaskScheduler::WP         iTaskScheduler;
};

}//namespace GPlatform

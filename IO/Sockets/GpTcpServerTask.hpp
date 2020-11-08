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
                                GpTcpServerTask     (GpIOEventPoller::WP        aIOPooler,
                                                     GpSocketTaskFactory::SP    aTaskFactory,
                                                     GpTaskScheduler::WP        aTaskScheduler,
                                                     GpSocketTCP::SP            aSocket) noexcept;
    virtual                     ~GpTcpServerTask    (void) noexcept override final;

    static  GpTcpServerTask::SP SConstruct          (const GpSocketAddr&        aAddr,
                                                     const GpSocketFlags&       aFlags,
                                                     const count_t              aMaxListenQueueSize,
                                                     GpIOEventPoller::SP        aIOPooler,
                                                     GpSocketTaskFactory::SP    aTaskFactory,
                                                     GpTaskScheduler::WP        aTaskScheduler);

protected:
    virtual void                OnStart             (void) override final;
    virtual GpTask::ResT        OnStep              (EventOptRefT aEvent) override final;
    virtual void                OnStop              (void) noexcept override final;

    virtual GpTask::ResT        OnSockReadyToRead   (GpSocket& aSocket) override final;
    virtual GpTask::ResT        OnSockReadyToWrite  (GpSocket& aSocket) override final;
    virtual void                OnSockClosed        (GpSocket& aSocket) override final;
    virtual void                OnSockError         (GpSocket& aSocket) override final;

private:
    GpSocketTaskFactory::SP     iTaskFactory;
    GpTaskScheduler::WP         iTaskScheduler;
};

}//namespace GPlatform

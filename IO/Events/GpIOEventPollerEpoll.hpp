#pragma once

#include "GpIOEventPoller.hpp"

#if defined(GP_OS_LINUX)

#include <sys/epoll.h>

namespace GPlatform {

class GPNETWORK_API GpIOEventPollerEpoll final: public GpIOEventPoller
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpIOEventPollerEpoll)
    CLASS_DECLARE_DEFAULTS(GpIOEventPollerEpoll)

    using EventT    = struct epoll_event;
    using EventsT   = GpVector<EventT>;

public:
                                GpIOEventPollerEpoll    (std::string_view       aName,
                                                         GpTaskFiberBarrier::SP aStartBarrier);
    virtual                     ~GpIOEventPollerEpoll   (void) noexcept override final;

    void                        Configure               (const milliseconds_t   aMaxStepTime,
                                                         const count_t          aMaxEventsCnt);

protected:
    virtual void                OnStart                 (void) override final;
    virtual GpTask::ResT        OnStep                  (EventOptRefT aEvent) override final;
    virtual void                OnStop                  (void) noexcept override final;

    virtual void                OnAddSubscriber         (GpEventSubscriber::SP  aSubscriber,
                                                         const GpIOObjectId     aIOObjectId) override final;
    virtual void                OnRemoveSubscriber      (const GpIOObjectId     aIOObjectId) override final;

private:
    milliseconds_t              iMaxStepTime;
    milliseconds_t              iNextStepTime;
    int                         iEpollId    = -1;
    EventsT                     iEvents;
};

}//GPlatform

#endif//#if defined(GP_OS_LINUX)

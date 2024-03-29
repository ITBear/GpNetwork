#pragma once

#include "../GpIOEventPoller.hpp"

#if defined(GP_OS_LINUX)

#include <sys/epoll.h>

namespace GPlatform {

class GpIOEventPollerEpoll final: public GpIOEventPoller
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpIOEventPollerEpoll)
    CLASS_DD(GpIOEventPollerEpoll)

    using EventT    = struct epoll_event;
    using EventsT   = std::vector<EventT>;

public:
    inline                              GpIOEventPollerEpoll    (std::u8string aName) noexcept;
    virtual                             ~GpIOEventPollerEpoll   (void) noexcept override final;

    void                                Configure               (const milliseconds_t   aMaxStepTime,
                                                                 const size_t           aMaxEventsCnt);

protected:
    virtual void                        OnStart                 (void) override final;
    virtual GpTaskRunRes::EnumT         OnStep                  (void) override final;
    virtual std::optional<GpException>  OnStop                  (void) noexcept override final;

    virtual void                        OnAddObject             (const GpIOObjectId aIOObjectId) override final;
    virtual void                        OnRemoveObject          (const GpIOObjectId aIOObjectId) override final;

private:
    milliseconds_t                      iMaxStepTime;
    milliseconds_t                      iNextStepTime;
    int                                 iEpollId    = -1;
    EventsT                             iEvents;
};

GpIOEventPollerEpoll::GpIOEventPollerEpoll (std::u8string aName) noexcept:
GpIOEventPoller(std::move(aName))
{
}

}//GPlatform

#endif//#if defined(GP_OS_LINUX)

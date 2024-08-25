#pragma once

#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp>

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
                                GpIOEventPollerEpoll    (std::string aName) noexcept;
    virtual                     ~GpIOEventPollerEpoll   (void) noexcept override final;

    void                        Configure               (milliseconds_t aMaxStepTime,
                                                         size_t         aMaxEventsCnt);

protected:
    virtual void                OnStart                 (void) override final;
    virtual GpTaskRunRes::EnumT OnStep                  (void) override final;
    virtual void                OnStop                  (StopExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void                OnStopException         (const GpException& aException) noexcept override final;

    virtual void                OnAddObject             (GpSocketId         aSocketId,
                                                         GpIOEventsTypes    aEventTypes) REQUIRES(iSpinLock) override final;
    virtual void                OnRemoveObject          (GpSocketId aSocketId) REQUIRES(iSpinLock) override final;

private:
    milliseconds_t              iMaxStepTime    GUARDED_BY(iSpinLock);
    int                         iEpollId        GUARDED_BY(iSpinLock) = -1;
    EventsT                     iEvents         GUARDED_BY(iSpinLock);
};

}// namespace GPlatform

#endif// #if defined(GP_OS_LINUX)

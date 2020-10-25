/*
#pragma once

#include "GpPollerIO.hpp"

#if defined(GP_OS_LINUX)

#include <sys/epoll.h>

namespace GPlatform {

class GPNETWORK_API GpEpoll final: public GpPollerIO
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpEpoll)
    CLASS_DECLARE_DEFAULTS(GpEpoll)

    using EventT    = struct epoll_event;
    using EventsT   = GpVector<EventT>;

public:
                                GpEpoll         (void) noexcept;
    virtual                     ~GpEpoll        (void) noexcept override final;

    virtual void                Init            (void) override final;

private:
    GpIODevice::DeviceIdT       iEpollId    = GpIODevice::DefaultId();
    EventsT                     iEpollEvents;
};

}//GPlatform

#endif//#if defined(GP_OS_LINUX)
*/

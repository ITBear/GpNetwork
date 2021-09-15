#include "GpIOEventPollerEpoll.hpp"

#if defined(GP_OS_LINUX)

#if defined(GP_POSIX)
#   include <unistd.h>
#endif

#if !defined(EPOLLRDHUP)
#   define EPOLLRDHUP 0x2000
#endif

#include <iostream>

namespace GPlatform {

GpIOEventPollerEpoll::GpIOEventPollerEpoll
(
    std::string_view        aName,
    GpTaskFiberBarrier::SP  aStartBarrier
):
GpIOEventPoller(aName, std::move(aStartBarrier))
{
}

GpIOEventPollerEpoll::~GpIOEventPollerEpoll (void) noexcept
{
    ReleaseStartBarrier();
}

void    GpIOEventPollerEpoll::Configure
(
    const milliseconds_t    aMaxStepTime,
    const count_t           aMaxEventsCnt
)
{
    THROW_GPE_COND(iEpollId == -1, "Already started"_sv);
    iMaxStepTime    = aMaxStepTime;
    iNextStepTime   = aMaxStepTime;
    iEvents.resize(aMaxEventsCnt.As<size_t>());
}

void    GpIOEventPollerEpoll::OnStart (void)
{
    GpIOEventPoller::OnStart();

    iEpollId = epoll_create1(EPOLL_CLOEXEC);
    THROW_GPE_COND(iEpollId >= 0, [&](){return GpErrno::SGetAndClear();});

    ReleaseStartBarrier();
}

GpTask::ResT    GpIOEventPollerEpoll::OnStep (EventOptRefT /*aEvent*/)
{
    EventT* eventsPtr = iEvents.data();

    //Poll wait...
    const int nfds = epoll_wait
    (
        iEpollId,
        eventsPtr,
        NumOps::SConvert<int>(iEvents.size()),
        NumOps::SConvert<int>(iNextStepTime.Value())
    );

    if (   (nfds < 0)
        && (errno == EINTR))
    {
        return GpTask::ResT::READY_TO_EXEC;
    }

    //Check for errors
    THROW_GPE_COND(nfds >= 0, [&](){return GpErrno::SGetAndClear();});

    if (nfds == 0)
    {
        return GpTask::ResT::READY_TO_EXEC;
    }

    //Process events
    const size_t    eventsCount = NumOps::SConvert<size_t>(nfds);
    const EventT*   epEvent     = eventsPtr;

    for (size_t id = 0; id < eventsCount; ++id)
    {
        const u_int_32 eventsMask = epEvent->events;

        GpEventSubscriber* subscriber = static_cast<GpEventSubscriber*>(epEvent->data.ptr);
        ++epEvent;

        GpIOEventsTypes ioEvents;

        if (eventsMask & EPOLLOUT)
        {
            ioEvents.Set(GpIOEventType::READY_TO_WRITE);
        }

        if ((eventsMask & EPOLLIN) || (eventsMask & EPOLLPRI))
        {
            ioEvents.Set(GpIOEventType::READY_TO_READ);
        }

        if ((eventsMask & EPOLLRDHUP) ||
            (eventsMask & EPOLLHUP))
        {
            ioEvents.Set(GpIOEventType::CLOSED);
        }

        if (eventsMask & EPOLLERR)
        {
            ioEvents.Set(GpIOEventType::ERROR_OCCURRED);
        }

        subscriber->PushEvent(MakeSP<GpIOEvent>(ioEvents));
    }

    return GpTask::ResT::READY_TO_EXEC;
}

void    GpIOEventPollerEpoll::OnStop (void) noexcept
{
    if (iEpollId >= 0)
    {
        close(iEpollId);
        iEpollId = -1;
    }

    GpIOEventPoller::OnStop();
}

void    GpIOEventPollerEpoll::OnAddSubscriber
(
    GpEventSubscriber::SP   aSubscriber,
    const GpIOObjectId      aIOObjectId
)
{
    const int fd = aIOObjectId.As<int>();

    EventT listenev;
    listenev.data.u64   = 0LL;
    listenev.data.fd    = fd;
    listenev.data.ptr   = aSubscriber.P();
    listenev.events     = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLHUP | EPOLLET;

    if (epoll_ctl(iEpollId, EPOLL_CTL_ADD, fd, &listenev) != 0)
    {
        THROW_GPE(GpErrno::SGetAndClear());
    }
}

void    GpIOEventPollerEpoll::OnRemoveSubscriber (const GpIOObjectId aIOObjectId)
{
    if (epoll_ctl(iEpollId, EPOLL_CTL_DEL, aIOObjectId.As<int>(), nullptr) != 0)
    {
        THROW_GPE(GpErrno::SGetAndClear());
    }
}

}//GPlatform

#endif//#if defined(GP_OS_LINUX)

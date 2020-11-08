#include "GpIOEventPollerEpoll.hpp"

#if defined(GP_OS_LINUX)

#if defined(GP_POSIX)
#   include <unistd.h>
#endif

#if !defined(EPOLLRDHUP)
#   define EPOLLRDHUP 0x2000
#endif

namespace GPlatform {

GpIOEventPollerEpoll::GpIOEventPollerEpoll (GpTaskFiberBarrierLock aStartDoneLock) noexcept:
GpIOEventPoller(std::move(aStartDoneLock))
{
}

GpIOEventPollerEpoll::~GpIOEventPollerEpoll (void) noexcept
{   
}

void    GpIOEventPollerEpoll::Configure (const milliseconds_t   aMaxStepTime,
                                         const count_t          aMaxEventsCnt)
{
    THROW_GPE_COND_CHECK_M(iEpollId == -1, "Already started"_sv);
    iMaxStepTime = aMaxStepTime;
    iEvents.resize(aMaxEventsCnt.As<size_t>());
}

void    GpIOEventPollerEpoll::OnStart (void)
{
    iEpollId = epoll_create1(EPOLL_CLOEXEC);
    THROW_GPE_COND_CHECK_M(iEpollId >= 0, GpErrno::SGetAndClear());

    GpIOEventPoller::OnStart();
}

GpTask::ResT    GpIOEventPollerEpoll::OnStep (EventOptRefT /*aEvent*/)
{
    EventT* eventsPtr = iEvents.data();

    //Poll wait...
    const int nfds = epoll_wait(iEpollId,
                                eventsPtr,
                                NumOps::SConvert<int>(iEvents.size()),
                                NumOps::SConvert<int>(iMaxStepTime.Value()));

    //Check for errors
    THROW_GPE_COND_CHECK_M(nfds >= 0, GpErrno::SGetAndClear());

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

        if ((eventsMask & EPOLLRDHUP) ||//detect peer shutdown (normal mode)
            (eventsMask & EPOLLHUP))    //unexpected close
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

void    GpIOEventPollerEpoll::OnAddSubscriber (GpEventSubscriber::SP    aSubscriber,
                                               const GpIOObjectId       aIOObjectId)
{
    const int fd = aIOObjectId.As<int>();

    EventT listenev;
    listenev.data.u64  = 0LL;
    listenev.data.fd   = fd;
    listenev.data.ptr  = aSubscriber.P();
    listenev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLHUP | EPOLLET;

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

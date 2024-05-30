#include "GpIOEventPollerEpoll.hpp"

#if defined(GP_OS_LINUX)

#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

#if defined(GP_POSIX)
#   include <unistd.h>
#endif

#if !defined(EPOLLRDHUP)
#   define EPOLLRDHUP 0x2000
#endif

namespace GPlatform {

GpIOEventPollerEpoll::~GpIOEventPollerEpoll (void) noexcept
{   
}

void    GpIOEventPollerEpoll::Configure
(
    const milliseconds_t    aMaxStepTime,
    const size_t            aMaxEventsCnt
)
{
    LOG_INFO
    (
        fmt::format
        (
            "[GpIOEventPollerEpoll::Configure]: {}",
            TaskName()
        )
    );

    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    THROW_COND_GP
    (
        iEpollId == -1,
        "Already started"_sv
    );

    iMaxStepTime = aMaxStepTime;
    iEvents.resize(aMaxEventsCnt);

    // Create epoll
    iEpollId = epoll_create1(EPOLL_CLOEXEC);

    THROW_COND_GP
    (
        iEpollId >= 0,
        []()
        {
            return std::string(GpErrno::SGetAndClear());
        }
    );
}

void    GpIOEventPollerEpoll::OnStart (void)
{
    LOG_INFO
    (
        fmt::format
        (
            "[GpIOEventPollerEpoll::OnStart]: {}",
            TaskName()
        )
    );
}

GpTaskRunRes::EnumT GpIOEventPollerEpoll::OnStep (void)
{
    EventT* eventsPtr   = nullptr;
    int     eventsSize  = 0;

    // Check
    int             epollId = -1;
    milliseconds_t  maxStepTime;
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

        eventsPtr   = std::data(iEvents);
        eventsSize  = NumOps::SConvert<int>(std::size(iEvents));

        if (eventsPtr == nullptr) [[unlikely]]
        {
            return GpTaskRunRes::READY_TO_RUN;
        }

        if (iEpollId == -1) [[unlikely]]
        {
            return GpTaskRunRes::READY_TO_RUN;
        }

        epollId     = iEpollId;
        maxStepTime = iMaxStepTime;
    }

    // Poll wait...
    const int nfds = epoll_wait
    (
        epollId,
        eventsPtr,
        eventsSize,
        NumOps::SConvert<int>(maxStepTime.Value())
    );

    if (   (nfds < 0)
        && (errno == EINTR)) [[unlikely]]
    {
        return GpTaskRunRes::READY_TO_RUN;
    }

    // Check for errors
    THROW_COND_GP
    (
        nfds >= 0,
        [&]()
        {
            return std::string(GpErrno::SGetAndClear());
        }
    );

    if (nfds == 0)
    {
        return GpTaskRunRes::READY_TO_RUN;
    }

    // Process events
    const size_t    eventsCount = NumOps::SConvert<size_t>(nfds);
    const EventT*   epEvent     = eventsPtr;

    for (size_t id = 0; id < eventsCount; ++id)
    {
        const EventT*       epEventCurrent  = epEvent++;
        const u_int_32      eventsMask      = epEventCurrent->events;
        const GpSocketId    fd              = epEventCurrent->data.fd;

        const GpIOEventsTypes::value_type events =
              GpIOEventsTypes::value_type((1 << GpIOEventType::READY_TO_WRITE) * bool(eventsMask & EPOLLOUT))
            | GpIOEventsTypes::value_type((1 << GpIOEventType::READY_TO_READ)  * bool((eventsMask & EPOLLIN) || (eventsMask & EPOLLPRI)))
            | GpIOEventsTypes::value_type((1 << GpIOEventType::CLOSED)         * bool((eventsMask & EPOLLRDHUP) || (eventsMask & EPOLLHUP)))
            | GpIOEventsTypes::value_type((1 << GpIOEventType::ERROR_OCCURRED) * bool(eventsMask & EPOLLERR));

        {
            GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
            ProcessEvents(fd, GpIOEventsTypes(events));
        }
    }

    return GpTaskRunRes::READY_TO_RUN;
}

GpException::C::Opt GpIOEventPollerEpoll::OnStop (void) noexcept
{
    LOG_INFO
    (
        fmt::format
        (
            "[GpIOEventPollerEpoll::OnStop]: {}",
            TaskName()
        )
    );

    GpException::C::Opt ex;

    try
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

        if (iEpollId >= 0)
        {
            close(iEpollId);
            iEpollId = -1;

            iEvents.clear();
            iEvents.shrink_to_fit();
        }
    } catch (const GpException& e)
    {
        ex = e;
    } catch (const std::exception& e)
    {
        ex = GpException(e.what());
    } catch (...)
    {
        ex = GpException("[GpIOEventPollerEpoll::OnStop]: unknown exception"_sv);
    }

    return ex;
}

void    GpIOEventPollerEpoll::OnAddObject (const GpSocketId aSocketId)
{
    const int fd = NumOps::SConvert<int>(aSocketId);

    EventT listenev;
    listenev.data.fd    = fd;
    listenev.events     = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLHUP | EPOLLET;

    if (epoll_ctl(iEpollId, EPOLL_CTL_ADD, fd, &listenev) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }   
}

void    GpIOEventPollerEpoll::OnRemoveObject (const GpSocketId aSocketId)
{
    if (aSocketId == GpSocketId_Default()) [[unlikely]]
    {
        return;
    }

    if (iEpollId < 0) [[unlikely]]
    {
        return;
    }       

    const int fd = NumOps::SConvert<int>(aSocketId);

    if (epoll_ctl(iEpollId, EPOLL_CTL_DEL, fd, nullptr) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }
}

}// GPlatform

#endif// #if defined(GP_OS_LINUX)

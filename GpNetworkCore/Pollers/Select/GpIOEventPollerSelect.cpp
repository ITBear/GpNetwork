#include <GpNetwork/GpNetworkCore/Pollers/Select/GpIOEventPollerSelect.hpp>

#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpNetwork/GpNetworkCore/GpNetworkErrors.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>

#include <iostream>

namespace GPlatform {

GpIOEventPollerSelect::GpIOEventPollerSelect (std::string aName) noexcept:
GpIOEventPoller{std::move(aName)}
{
}

GpIOEventPollerSelect::~GpIOEventPollerSelect (void) noexcept
{
}

void    GpIOEventPollerSelect::Configure (const milliseconds_t aMaxStepTime)
{
    LOG_INFO
    (
        fmt::format
        (
            "[GpIOEventPollerSelect::Configure]: {}",
            TaskName()
        )
    );

    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    iMaxStepTime = aMaxStepTime;
}

void    GpIOEventPollerSelect::OnStart (void)
{   
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    FD_ZERO(&iFdSetMaster);
    FD_ZERO(&iReadFdSetWorking);
    FD_ZERO(&iWriteFdSetWorking);
    FD_ZERO(&iErrorFdSetWorking);

    LOG_INFO
    (
        fmt::format
        (
            "[GpIOEventPollerSelect::OnStart]: {}",
            TaskName()
        )
    );
}

GpTaskRunRes::EnumT GpIOEventPollerSelect::OnStep (void)
{
    milliseconds_t maxStepTime;
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
        maxStepTime = iMaxStepTime;
    }

    // Copy iFdSetMaster to READ, WRITE, ERROR sets
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

        std::memcpy(&iReadFdSetWorking,  &iFdSetMaster, sizeof(fd_set));
        std::memcpy(&iWriteFdSetWorking, &iFdSetMaster, sizeof(fd_set));
        std::memcpy(&iErrorFdSetWorking, &iFdSetMaster, sizeof(fd_set));
    }

    // Setup timeout
    struct timeval timeout;
    {
        if (maxStepTime < 1000.0_si_ms)
        {
            timeout.tv_sec  = 0;
            timeout.tv_usec = static_cast<decltype(timeval::tv_usec)>(microseconds_t(maxStepTime).Value());
        } else
        {
            const double seconds    = floor(seconds_t(maxStepTime).Value());
            const double useconds   = (microseconds_t(maxStepTime) - microseconds_t::SMake(seconds)).Value();

            timeout.tv_sec  = static_cast<decltype(timeval::tv_sec)>(seconds);
            timeout.tv_usec = static_cast<decltype(timeval::tv_usec)>(useconds);
        }
    }

    {
        bool socketsIsEmpty = false;
        {
            GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
            socketsIsEmpty = iSockets.empty();
        }

        if (socketsIsEmpty) [[unlikely]]
        {
            select
            (
                0,
                nullptr,
                nullptr,
                nullptr,
                &timeout
            );

            return GpTaskRunRes::READY_TO_RUN;
        }
    }

    // Do select
    int maxSocketId = -1;
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
        maxSocketId = iMaxSocketId;
    }

    const int selectRes = select
    (
        maxSocketId + 1,
        &iReadFdSetWorking,
        &iWriteFdSetWorking,
        &iErrorFdSetWorking,
        &timeout
    );

    // Check for errors
    THROW_COND_GP
    (
        selectRes >= 0,
        []()
        {
            return GpNetworkErrors::SGetLastError();
        }
    );

    if (selectRes == 0)
    {
        return GpTaskRunRes::READY_TO_RUN;
    }

    // Copy iSockets
    SocketsSetT socketsCopy;
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
        socketsCopy = iSockets;
    }

    // Process events
    for (const auto& [socketId, eventTypes]: socketsCopy)
    {
        GpIOEventsTypes events;

        // Check for in ready for read set
        if (FD_ISSET(socketId, &iReadFdSetWorking))
        {
            events |= GpIOEventType::READY_TO_READ;
        }

        // Check for closed
        if (!GpSocketTCP::SIsConnected(socketId))
        {
            events |= GpIOEventType::CLOSED;
        }

        // Check for in ready for write set
        if (   FD_ISSET(socketId, &iWriteFdSetWorking)
            && (!events.Test(GpIOEventType::CLOSED)))
        {
            // TODO: add flag to socket
            events |= GpIOEventType::READY_TO_WRITE;
        }

        // Check for in error set
        if (FD_ISSET(socketId, &iErrorFdSetWorking))
        {
            events |= GpIOEventType::ERROR_OCCURRED;
        }

        events.ApplyMask(eventTypes);

        if (!events.Empty())
        {
            GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
            ProcessEvents(socketId, events);
        }
    }// while (testIterFn(socketsIter))

    return GpTaskRunRes::READY_TO_RUN;
}

void    GpIOEventPollerSelect::OnStop (StopExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        LOG_INFO
        (
            fmt::format
            (
                "[GpIOEventPollerSelect::OnStop]: {}",
                TaskName()
            )
        );

        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

        iSockets.clear();
    } catch (const GpException& e)
    {
        aStopExceptionsOut.emplace_back(e);
    } catch (const std::exception& e)
    {
        aStopExceptionsOut.emplace_back(GpException{e.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpIOEventPollerSelect::OnStop]: unknown exception"_sv});
    }

    GpIOEventPoller::OnStop(aStopExceptionsOut);
}

void    GpIOEventPollerSelect::OnStopException (const GpException& aException) noexcept
{
    LOG_EXCEPTION
    (
        "[GpIOEventPollerSelect::OnStopException]",
        aException
    );
}

void    GpIOEventPollerSelect::OnAddObject
(
    const GpSocketId    aSocketId,
    GpIOEventsTypes     aEventTypes
)
{
    // Check for max count
    THROW_COND_GP
    (
        aSocketId != GpSocketId_Default(),
        "Wrong socket ID"
    );

    // Check for max count
    THROW_COND_GP
    (
        std::size(iSockets) < FD_SETSIZE,
        []()
        {
            return fmt::format
            (
                "The maximum socket count has been reached (FD_SETSIZE = {})",
                FD_SETSIZE
            );
        }
    );

    // Add to iSockets
    iSockets.emplace(aSocketId, aEventTypes.Value());

    // Add to iFdSetMaster
    FD_SET(aSocketId, &iFdSetMaster);

    // Update iMaxSocketId
    if (aSocketId > iMaxSocketId)
    {
        iMaxSocketId = NumOps::SConvert<decltype(iMaxSocketId)>(aSocketId);
    }
}

void    GpIOEventPollerSelect::OnRemoveObject (const GpSocketId aSocketId)
{
    if (aSocketId == GpSocketId_Default()) [[unlikely]]
    {
        return;
    }

    // Add to iSockets
    iSockets.erase(aSocketId);

    // Add to iFdSetMaster
    FD_CLR(aSocketId, &iFdSetMaster);

    // Update iMaxSocketId
    if (!iSockets.empty())
    {
        iMaxSocketId = NumOps::SConvert<decltype(iMaxSocketId)>((std::end(iSockets)--)->first);
    } else
    {
        iMaxSocketId = 0;
    }
}

}// GPlatform

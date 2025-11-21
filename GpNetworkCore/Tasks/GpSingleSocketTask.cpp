#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTask.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>

namespace GPlatform {

GpSingleSocketTask::GpSingleSocketTask (GpSocket::UP aSocketUP) noexcept:
iSocketUP{std::move(aSocketUP)}
{
}

GpSingleSocketTask::GpSingleSocketTask
(
    GpSocket::UP    aSocketUP,
    std::string     aTaskName
) noexcept:
GpSocketTask{std::move(aTaskName)},
iSocketUP{std::move(aSocketUP)}
{
}

GpSingleSocketTask::~GpSingleSocketTask (void) noexcept
{
}

void    GpSingleSocketTask::PushSocketEvents
(
    [[maybe_unused]] const GpSocketId   aSocketId,
    const GpIOEventsTypes               aIoEvents
)
{
    // Update iFiringIOEvents
    iFiringIOEvents.fetch_or(aIoEvents.RawValue(), std::memory_order_acq_rel);
}

GpTaskRunRes::EnumT GpSingleSocketTask::OnStep (void)
{
    // Fetch iFiringIOEvents and reset
    GpIOEventsTypes firingIOEvents{iFiringIOEvents.fetch_and(0, std::memory_order_acq_rel)};

    if (firingIOEvents.Empty())
    {
        return GpTaskRunRes::WAIT;
    }

    {
        GpUniqueLock uniqueLock{iSpinLock};
        ProcessSocketEvents(*iSocketUP, firingIOEvents);
    }

    return GpTaskRunRes::WAIT;
}

void    GpSingleSocketTask::ProcessSocketEvents
(
    GpSocket&       aSocket,
    GpIOEventsTypes aIoEvents
)
{
    if (aIoEvents.Test(GpIOEventType::READY_TO_READ))
    {
        OnReadyToRead(aSocket);
    }

    if (aIoEvents.Test(GpIOEventType::READY_TO_WRITE))
    {
        OnReadyToWrite(aSocket);
    }

    if (aIoEvents.Test(GpIOEventType::CLOSED)) [[unlikely]]
    {
        OnClosed(aSocket);
    } else if (aIoEvents.Test(GpIOEventType::ERROR_OCCURRED)) [[unlikely]]
    {
        OnError(aSocket);
    }
}

}// namespace GPlatform

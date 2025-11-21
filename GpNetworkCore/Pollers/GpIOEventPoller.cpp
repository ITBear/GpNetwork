#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpSocketTask.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>

namespace GPlatform {

GpIOEventPoller::GpIOEventPoller (std::string aName) noexcept:
GpTaskFiber{std::move(aName)}
{
}

GpIOEventPoller::~GpIOEventPoller (void) noexcept
{
}

void    GpIOEventPoller::AddSubscription
(
    const GpSocketId        aSocketId,
    GpTask::WP              aTaskWP,
    const GpIOEventsTypes   aEventTypes
)
{
    GpUniqueLock uniqueLock{iSpinLock};

    VERIFY
    (
        iSocketsByTask.contains(aSocketId) == false,
        [aSocketId]()
        {
            return fmt::format
            (
                "Already subscripted for socket {}",
                aSocketId
            );
        }
    );

    // Verify task type (task must be GpSocketsTask)
    {
        GpTask::SP taskSP = aTaskWP.Lock();

        VERIFY
        (
            taskSP.IsNotNULL(),
            "The task does not exist"
        );

        VERIFY
        (
            taskSP.Vn().TypeUID() == GpSocketTask::STypeUID(),
            []()
            {
                return fmt::format("Wrong task type, expected {}", GpSocketTask::STypeUID());
            }
        );
    }

    iSocketsByTask.emplace(aSocketId, std::move(aTaskWP));

    try
    {
        OnAddSocket(aSocketId, aEventTypes);
    } catch (...)
    {
        iSocketsByTask.erase(aSocketId);
        throw;
    }
}

bool    GpIOEventPoller::RemoveSubscription (const GpSocketId aSocketId)
{
    GpUniqueLock uniqueLock{iSpinLock};

    auto iter = iSocketsByTask.find(aSocketId);

    if (iter != std::end(iSocketsByTask)) [[likely]]
    {
        iSocketsByTask.erase(iter);
        OnRemoveSocket(aSocketId);

        return true;
    }

    return false;
}

bool    GpIOEventPoller::ProcessEvents
(
    const GpSocketId    aSocketId,
    GpIOEventsTypes     aEvents
)
{
    auto iter = iSocketsByTask.find(aSocketId);

    if (iter == std::end(iSocketsByTask))
    {
        return false;
    }   

    GpTask::WP& taskWP = iter->second;
    GpTask::SP  taskSP = taskWP.Lock();

    if (taskSP.IsNULL())
    {
        return false;
    }

    GpSocketTask& socketTask = static_cast<GpSocketTask&>(taskSP.Vn());
    socketTask.PushSocketEvents(aSocketId, aEvents);
    WAKEUP_TASK(socketTask);

    if (   aEvents.Test(GpIOEventType::CLOSED)
        || aEvents.Test(GpIOEventType::ERROR_OCCURRED)) [[unlikely]]
    {
        iSocketsByTask.erase(aSocketId);
        OnRemoveSocket(aSocketId);
    }

    return true;
}

void    GpIOEventPoller::OnStart (void)
{
    // NOP
}

void    GpIOEventPoller::OnStop ([[maybe_unused]] ExceptionsT& aStopExceptionsOut) noexcept
{
    // NOP
}

}// namespace GPlatform

#include "GpIOEventPollerCatalog.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/Config/IncludeExt/fmt.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

#if defined(GP_OS_LINUX)
#   include <GpNetwork/GpNetworkCore/Pollers/Epoll/GpIOEventPollerEpollFactory.hpp>
#   include <GpNetwork/GpNetworkCore/Pollers/Epoll/GpIOEventPollerEpollCfgDesc.hpp>
#endif

#include <GpNetwork/GpNetworkCore/Pollers/Select/GpIOEventPollerSelectFactory.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/Select/GpIOEventPollerSelectCfgDesc.hpp>

namespace GPlatform {

GpIOEventPollerCatalog  GpIOEventPollerCatalog::sInstance;

GpIOEventPollerCatalog::GpIOEventPollerCatalog (void) noexcept
{
}

GpIOEventPollerCatalog::~GpIOEventPollerCatalog (void) noexcept
{
}

bool    GpIOEventPollerCatalog::SAddSubscriptionSafe
(
    const GpSocketId            aSocketId,
    const GpTaskId              aSocketTaskId,
    const GpIOEventPollerIdx    aIoEventPollerIdx
)
{
    // Get event poller
    GpIOEventPoller::C::Opt::SP eventPollerOpt = GpIOEventPollerCatalog::S().GetByIdxOpt(aIoEventPollerIdx);

    if (eventPollerOpt.has_value()) [[likely]]
    {
        eventPollerOpt->Vn().AddSubscription
        (
            aSocketId,
            aSocketTaskId,
            [](const GpTaskId aTaskId, const GpIOEventPoller::SubsriberResValT aIOEventsTypes)
            {
                GpTaskScheduler::S().MakeTaskReady(aTaskId, aIOEventsTypes);
            }
        );

        return true;
    }

    return false;
}

bool    GpIOEventPollerCatalog::SRemoveSubscriptionSafe
(
    const GpSocketId            aSocketId,
    const GpTaskId              aSocketTaskId,
    const GpIOEventPollerIdx    aIoEventPollerIdx
)
{
    // Get event poller
    GpIOEventPoller::C::Opt::SP eventPollerOpt = GpIOEventPollerCatalog::S().GetByIdxOpt(aIoEventPollerIdx);

    if (eventPollerOpt.has_value()) [[likely]]
    {
        eventPollerOpt->Vn().RemoveSubscription(aSocketId, aSocketTaskId);
        return true;
    }

    return false;
}

GpIOEventPoller::C::Opt::SP GpIOEventPollerCatalog::GetByIdxOpt (const GpIOEventPollerIdx aIoEventPollerIdx) noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    if (aIoEventPollerIdx.Value() >= std::size(iCatalogIdxToPoller))
    {
        return std::nullopt;
    }

    return iCatalogIdxToPoller[aIoEventPollerIdx.Value()];
}

GpIOEventPoller::SP GpIOEventPollerCatalog::GetByIdx (const GpIOEventPollerIdx aIoEventPollerIdx)
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    GpIOEventPoller::C::Opt::SP eventPollerOpt = GetByIdxOpt(aIoEventPollerIdx);

    THROW_COND_GP
    (
        eventPollerOpt.has_value(),
        [&]() REQUIRES(iSpinLockRW)
        {
            return fmt::format
            (
                "Event poller not found by name '{}'",
                NameByIdx(aIoEventPollerIdx)
            );
        }
    );

    return eventPollerOpt.value();
}

GpIOEventPoller::C::Opt::SP GpIOEventPollerCatalog::GetByNameOpt (std::string_view aName) noexcept
{
    const GpIOEventPollerIdx idx = IdxByName(aName);
    return GetByIdxOpt(idx);
}

GpIOEventPoller::SP GpIOEventPollerCatalog::GetByName (std::string_view aName)
{
    const GpIOEventPollerIdx idx = IdxByName(aName);
    return GetByIdx(idx);
}

GpIOEventPollerIdx  GpIOEventPollerCatalog::IdxByName (std::string_view aName)
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    const auto iter = iCatalogNameToIdx.find(aName);

    if (iter != std::end(iCatalogNameToIdx))
    {
        return iter->second;
    }

    THROW_GP
    (
        fmt::format
        (
            "IO event poller not found by name {}",
            aName
        )
    );
}

void    GpIOEventPollerCatalog::Start (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

#if defined(GP_OS_LINUX)
    // epoll
    RegisterPollerType
    (
        GpIOEventPollerEpollCfgDesc::SReflectModelUid(),
        [](std::string_view aName, const GpIOEventPollerCfgDesc& aCfg) -> GpIOEventPoller::SP
        {
            const GpIOEventPollerEpollCfgDesc& cfg = GpReflectManager::SCastCRef<GpIOEventPollerEpollCfgDesc>(aCfg);

            return GpIOEventPollerEpollFactory
            (
                cfg.max_step_time,
                cfg.max_events_cnt
            ).NewInstance("IO event poller: '"_sv + aName + "'"_sv);
        }
    );
#endif

    // select
    RegisterPollerType
    (
        GpIOEventPollerSelectCfgDesc::SReflectModelUid(),
        [](std::string_view aName, const GpIOEventPollerCfgDesc& aCfg) -> GpIOEventPoller::SP
        {
            const GpIOEventPollerSelectCfgDesc& cfg = GpReflectManager::SCastCRef<GpIOEventPollerSelectCfgDesc>(aCfg);

            return GpIOEventPollerSelectFactory
            (
                cfg.max_step_time
            ).NewInstance("IO event poller: '"_sv + aName + "'"_sv);
        }
    );

    for (const auto&[pollerName, pollerCfg]: aCfgs)
    {
        // Try to find poller type
        const auto pollerTypeIter = iRegisteredPollerTypes.find(pollerCfg->ReflectModelUid());

        if (pollerTypeIter == std::end(iRegisteredPollerTypes))
        (
            THROW_GP
            (
                fmt::format
                (
                    "Unknown IO event poller '{}', config type UID = '{}'",
                    pollerName,
                    pollerCfg->ReflectModelUid().ToString()
                )
            )
        );

        // Create new poller
        GpIOEventPoller::SP ioEventPoller = pollerTypeIter->second(pollerName, pollerCfg.V());

        // Start
        GpTaskFiber::StartFutureT::SP startFuture = ioEventPoller->GetStartFuture();
        GpTaskScheduler::S().NewToReady(ioEventPoller);

        // Wait for started
        if (startFuture->WaitFor(2000.0_si_ms) == false)
        {
            ioEventPoller->RequestStop();

            THROW_GP
            (
                fmt::format
                (
                    "Failed to start event poller '{}', timeout",
                    pollerName
                )
            );
        }

        // Check start result
        const std::string pollerNameStr{pollerName};

        GpTaskFiber::StartFutureT::SCheckIfReady
        (
            startFuture.V(),
            [&](typename GpTaskFiber::StartFutureT::value_type&) REQUIRES(iSpinLockRW) // OnSuccessFnT
            {
                const GpIOEventPollerIdx idx = AddNewName(pollerNameStr);

                iCatalogIdxToPoller.resize(idx.Value() + 1);
                iCatalogIdxToPoller[idx.Value()] = std::move(ioEventPoller);
            },
            [](const GpException& aEx)// OnExceptionFnT
            {
                throw aEx;
            }
        );
    }
}

void    GpIOEventPollerCatalog::StopAndClear (void)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    iCatalogNameToIdx.clear();
    iRegisteredPollerTypes.clear();

    for (GpIOEventPoller::SP& ioEventPoller: iCatalogIdxToPoller)
    {
        // Send stop
        GpTask::DoneFutureT::SP doneFuture = GpTaskScheduler::S().RequestStop(ioEventPoller.V());

        // Wait for stop
        if (doneFuture->WaitFor(2000.0_si_ms) == false)
        {
            LOG_ERROR
            (
                fmt::format
                (
                    "Failed to stop io event poller '{}', timeout",
                    ioEventPoller->TaskName()
                )
            );

            continue;
        }
    }

    iCatalogIdxToPoller.clear();
}

void    GpIOEventPollerCatalog::RegisterPollerType
(
    const GpUUID&   aUid,
    CreatePollerFnT aFn
)
{
    iRegisteredPollerTypes.emplace(aUid, std::move(aFn));
}

std::string_view    GpIOEventPollerCatalog::NameByIdx (const GpIOEventPollerIdx aIoEventPollerIdx) noexcept
{
    for (const auto&[name, idx]: iCatalogNameToIdx)
    {
        if (idx == aIoEventPollerIdx)
        {
            return name;
        }
    }

    return {};
}

GpIOEventPollerIdx  GpIOEventPollerCatalog::AddNewName (std::string aName)
{
    // Check if name is unique
    THROW_COND_GP
    (
        iCatalogNameToIdx.count(aName) == false,
        [&aName]()
        {
            return fmt::format
            (
                "IO event poller name '{}' is not unique",
                aName
            );
        }
    );

    // Add new name
    const size_t idx = std::size(iCatalogIdxToPoller);
    iCatalogNameToIdx[aName] = idx;

    return GpIOEventPollerIdx{idx};
}

}// namespace GPlatform

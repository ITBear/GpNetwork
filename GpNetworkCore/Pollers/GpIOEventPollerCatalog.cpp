#include "GpIOEventPollerCatalog.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>

#if defined(GP_OS_LINUX)
#   include "Epoll/GpIOEventPollerEpollFactory.hpp"
#   include "Epoll/GpIOEventPollerEpollCfgDesc.hpp"
#endif

#include <fmt/core.h>

namespace GPlatform {

GpIOEventPollerCatalog  GpIOEventPollerCatalog::sInstance;

GpIOEventPollerCatalog::GpIOEventPollerCatalog (void) noexcept
{
}

GpIOEventPollerCatalog::~GpIOEventPollerCatalog (void) noexcept
{
}

GpIOEventPollerCatalog::CatalogT::ValueOptT GpIOEventPollerCatalog::GetOpt (std::u8string_view aPollerName) noexcept
{
    return iCatalog.GetOpt(aPollerName);
}

GpIOEventPoller::SP GpIOEventPollerCatalog::Get (std::u8string_view aPollerName)
{
    CatalogT::ValueOptT eventPollerOpt = iCatalog.GetOpt(aPollerName);

    THROW_COND_GP
    (
        eventPollerOpt.has_value(),
        [aPollerName]()
        {
            return fmt::format
            (
                "Event poller not found by name '{}'",
                GpUTF::S_As_STR(aPollerName)
            );
        }
    );

    return eventPollerOpt.value();
}

void    GpIOEventPollerCatalog::Start (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs)
{
#if defined(GP_OS_LINUX)
    //Linux: epoll
    RegisterPollerType
    (
        GpIOEventPollerEpollCfgDesc::SReflectModelUid(),
        [](std::u8string_view aName, const GpIOEventPollerCfgDesc& aCfg) -> GpIOEventPoller::SP
        {
            const GpIOEventPollerEpollCfgDesc& epollCfg = GpReflectManager::SCastCRef<GpIOEventPollerEpollCfgDesc>(aCfg);

            return GpIOEventPollerEpollFactory
            (
                epollCfg.max_step_time,
                epollCfg.max_events_cnt
            ).NewInstance(u8"IO event poller: '"_sv + aName + u8"'"_sv);
        }
    );
#endif

    for (const auto&[pollerName, pollerCfg]: aCfgs)
    {
        // Try to find poller type
        const auto pollerTypeIter = iRegisteredPollerTypes.find(pollerCfg->ReflectModelUid());

        if (pollerTypeIter == iRegisteredPollerTypes.end())
        (
            THROW_GP(u8"Unknown IO event poller '" + pollerName + u8"', config type UID ="_sv + pollerCfg->ReflectModelUid().ToString())
        );

        // Create new poller
        GpIOEventPoller::SP poller = pollerTypeIter->second(pollerName, pollerCfg.V());

        // Start
        GpTaskFiber::StartFutureT::SP startFuture = poller->GetStartFuture();
        GpTaskScheduler::S().NewToReady(poller);

        // Wait for started
        if (startFuture->WaitFor(2000.0_si_ms) == false)
        {
            poller->UpStopRequestFlag();
            GpTaskScheduler::S().MakeTaskReady(poller->Id());
            THROW_GP(u8"Failed to start event poller '" + pollerName + u8"', timeout"_sv);
        }

        std::u8string pollerNameStr(pollerName);

        // Check start result
        GpTaskFiber::StartFutureT::SCheckIfReady
        (
            startFuture.V(),
            [&](typename GpTaskFiber::StartFutureT::value_type&)//OnSuccessFnT
            {
                iCatalog.Set
                (
                    std::move(pollerNameStr),
                    std::move(poller)
                );
            },
            [](const GpException& aEx)//OnExceptionFnT
            {
                throw aEx;
            }
        );
    }
}

void    GpIOEventPollerCatalog::StopAndClear (void)
{
    /*iCatalog.Apply
    (
        [](GpIOEventPoller::SP& aPoller)
        {
            aPoller->UpStopRequestFlag();
            GpTaskScheduler::S().MakeTaskReady(aPoller->Id());
        }
    );*/

    iCatalog.Clear();
    iRegisteredPollerTypes.clear();
}

void    GpIOEventPollerCatalog::RegisterPollerType
(
    const GpUUID&   aUid,
    CreatePollerFnT aFn
)
{
    iRegisteredPollerTypes.emplace(aUid, std::move(aFn));
}

}//namespace GPlatform

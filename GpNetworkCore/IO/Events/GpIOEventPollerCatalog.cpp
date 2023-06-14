#include "GpIOEventPollerCatalog.hpp"
#include "GpIOEventPollerEpoll.hpp"

namespace GPlatform {

GpIOEventPollerCatalog  GpIOEventPollerCatalog::sInstance;

GpIOEventPollerCatalog::GpIOEventPollerCatalog (void) noexcept
{
}

GpIOEventPollerCatalog::~GpIOEventPollerCatalog (void) noexcept
{
}

void    GpIOEventPollerCatalog::Start (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs)
{
    for (const auto& iter: aCfgs)
    {
        const auto&             cfgDesc     = iter.second.V();
        GpIOEventPoller::SP     eventPoller;
        StartItcPromiseT        startPromise;
        StartItcFutureT::SP     startFuture = startPromise.Future(GpTask::SCurrentUID());

        if (cfgDesc.epoll.IsNotNULL())
        {
            const auto& cfg = cfgDesc.epoll.V();

            GpIOEventPollerEpoll::SP ep = MakeSP<GpIOEventPollerEpoll>
            (
                u8"IO event poller: '"_sv + iter.first + u8"'"_sv,
                std::move(startPromise)
            );

            ep.Vn().Configure(cfg.max_step_time, cfg.max_events_cnt);

            eventPoller = std::move(ep);
        } else
        {
            THROW_GP(u8"Unknown IO event poller type"_sv);
        }

        std::ignore = GpTaskScheduler::S().NewToReadyDepend(eventPoller);

        startFuture.V().Wait();

        Register(iter.first, eventPoller);
    }
}

void    GpIOEventPollerCatalog::Clear (void)
{
    iCatalog.Clear();
}

GpIOEventPoller::SP GpIOEventPollerCatalog::Find (std::u8string_view aName)
{
    auto pollerOpt = iCatalog.GetCopyOpt(aName);

    THROW_COND_GP
    (
        pollerOpt.has_value(),
        [aName](){return u8"Event poller not found by name '"_sv + aName + u8"'"_sv;}
    );

    return pollerOpt.value();
}

void    GpIOEventPollerCatalog::Register
(
    std::u8string       aName,
    GpIOEventPoller::SP aEventPoller
)
{
    iCatalog.Set
    (
        std::move(aName),
        std::move(aEventPoller)
    );
}

}//namespace GPlatform

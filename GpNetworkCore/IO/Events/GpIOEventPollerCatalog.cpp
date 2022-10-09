#include "GpIOEventPollerCatalog.hpp"
#include "GpIOEventPollerEpoll.hpp"

namespace GPlatform {

GpIOEventPollerCatalog  GpIOEventPollerCatalog::sCatalog;

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
        const auto&         cfgDesc = iter.second.V();
        GpIOEventPoller::SP eventPoller;
        GpItcPromise        startPromise;
        GpItcFuture::SP     startFuture     = startPromise.Future();

        if (cfgDesc.epoll.IsNotNULL())
        {
            const auto& cfg = cfgDesc.epoll.V();

            GpIOEventPollerEpoll::SP ep = MakeSP<GpIOEventPollerEpoll>
            (
                "IO event poller: '"_sv + iter.first + "'"_sv,
                std::move(startPromise)
            );

            ep.Vn().Configure(cfg.max_step_time, cfg.max_events_cnt);

            eventPoller = std::move(ep);
        } else
        {
            THROW_GP("Unknown IO event poller type"_sv);
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

GpIOEventPoller::SP GpIOEventPollerCatalog::Find (std::string_view aName)
{
    return iCatalog.FindRetCopy(aName);
}

void    GpIOEventPollerCatalog::Register
(
    std::string         aName,
    GpIOEventPoller::SP aEventPoller
)
{
    iCatalog.Register
    (
        std::move(aName),
        std::move(aEventPoller)
    );
}

}//namespace GPlatform

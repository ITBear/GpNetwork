#include "GpIOEventPollerCatalog.hpp"
#include "GpIOEventPollerEpoll.hpp"

#include <iostream>

namespace GPlatform {

static int _GpIOEventPollerCatalog_counter = 0;

GpIOEventPollerCatalog  GpIOEventPollerCatalog::sCatalog;

GpIOEventPollerCatalog::GpIOEventPollerCatalog (void) noexcept
{
    _GpIOEventPollerCatalog_counter++;
    std::cout << "[GpIOEventPollerCatalog::GpIOEventPollerCatalog]: counter = " << _GpIOEventPollerCatalog_counter << std::endl;
}

GpIOEventPollerCatalog::~GpIOEventPollerCatalog (void) noexcept
{
    _GpIOEventPollerCatalog_counter--;
    std::cout << "[GpIOEventPollerCatalog::~GpIOEventPollerCatalog]: counter = " << _GpIOEventPollerCatalog_counter << std::endl;
}

void    GpIOEventPollerCatalog::StartFromConfig (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs)
{
    for (const auto& iter: aCfgs)
    {
        const auto&             cfgDesc = iter.second.VC();
        GpIOEventPoller::SP     eventPoller;
        GpTaskFiberBarrier::SP  satartBarrier = MakeSP<GpTaskFiberBarrier>(1_cnt);

        if (cfgDesc.epoll.IsNotNULL())
        {
            auto cfg = cfgDesc.epoll.VC();

            GpIOEventPollerEpoll::SP ep = MakeSP<GpIOEventPollerEpoll>
            (
                "IO event poller: '"_sv + iter.first + "'"_sv,
                satartBarrier
            );

            ep->Configure(cfg.max_step_time, cfg.max_events_cnt);

            eventPoller = ep;
        } else
        {
            THROW_GPE("Unknown IO event poller type"_sv);
        }

        GpTaskScheduler::SCurrentScheduler().value()->AddTaskToReady(eventPoller);

        Register(iter.first, eventPoller);

        satartBarrier->Wait();
    }
}

void    GpIOEventPollerCatalog::Clear (void)
{
    iCatalog.Clear();
}

GpIOEventPoller::WP GpIOEventPollerCatalog::Find (std::string_view aName)
{
    return iCatalog.Find(aName);
}

void    GpIOEventPollerCatalog::Register
(
    std::string_view    aName,
    GpIOEventPoller::SP aEventPoller
)
{
    iCatalog.Register(aName, std::move(aEventPoller));
}

}//namespace GPlatform

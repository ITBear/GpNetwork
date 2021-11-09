#include "GpIOEventPoller.hpp"

namespace GPlatform {

GpIOEventPoller::GpIOEventPoller
(
    std::string_view        aName,
    GpTaskFiberBarrier::SP  aStartBarrier
):
GpTaskFiberBase(aName),
iStartBarrier(std::move(aStartBarrier))
{
}

GpIOEventPoller::~GpIOEventPoller (void) noexcept
{
    ReleaseStartBarrier();
}

void    GpIOEventPoller::AddSubscriber
(
    GpEventSubscriber::SP   aSubscriber,
    const GpIOObjectId      aIOObjectId
)
{
    std::scoped_lock lock(iSubscribersLock);

    auto[e, r] = iSubscribers.emplace(aIOObjectId.RawValue(), aSubscriber);
    THROW_GPE_COND(r, "ID already added"_sv);

    try
    {
        OnAddSubscriber(aSubscriber, aIOObjectId);
    } catch(...)
    {
        iSubscribers.erase(aIOObjectId.RawValue());
        throw;
    }
}

void    GpIOEventPoller::RemoveSubscriber (const GpIOObjectId aIOObjectId)
{
    std::scoped_lock lock(iSubscribersLock);

    auto iter = iSubscribers.find(aIOObjectId.RawValue());

    THROW_GPE_COND(iter != iSubscribers.end(), "ID not found"_sv);
    iSubscribers.erase(iter);
    OnRemoveSubscriber(aIOObjectId);
}

void    GpIOEventPoller::OnStart (void)
{
    //NOP
}

void    GpIOEventPoller::OnStop  (void) noexcept
{
    std::scoped_lock lock(iSubscribersLock);
    iSubscribers.clear();
    ReleaseStartBarrier();
}

void    GpIOEventPoller::ReleaseStartBarrier (void) noexcept
{
    if (iStartBarrier.IsNotNULL())
    {
        iStartBarrier->Release(std::nullopt);
        iStartBarrier.Clear();
    }
}

}//GPlatform

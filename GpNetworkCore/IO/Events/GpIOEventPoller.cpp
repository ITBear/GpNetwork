#include "GpIOEventPoller.hpp"

namespace GPlatform {

GpIOEventPoller::~GpIOEventPoller (void) noexcept
{
    CompleteStartPromise(MakeSP<GpItcResult>());
}

void    GpIOEventPoller::AddSubscriber
(
    GpEventSubscriber::SP   aSubscriber,
    const GpIOObjectId      aIOObjectId
)
{
    std::scoped_lock lock(iSubscribersLock);

    THROW_COND_GP
    (
        iSubscribers.try_emplace(aIOObjectId.RawValue(), aSubscriber).second,
        "ID already added"_sv
    );

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

    THROW_COND_GP(iter != iSubscribers.end(), "ID not found"_sv);
    iSubscribers.erase(iter);
    OnRemoveSubscriber(aIOObjectId);
}

void    GpIOEventPoller::OnStop  (void) noexcept
{
    std::scoped_lock lock(iSubscribersLock);
    iSubscribers.clear();

    CompleteStartPromise(MakeSP<GpItcResult>());
}

}//GPlatform

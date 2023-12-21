#include "GpIOEventPoller.hpp"

namespace GPlatform {

GpIOEventPoller::~GpIOEventPoller (void) noexcept
{   
}

void    GpIOEventPoller::AddSubscription
(
    const GpIOObjectId                      aIOObjectId,
    const GpTaskId                          aTaskId,
    SubsribersEventChannelT::CallbackFnT&&  aFn
)
{
    std::scoped_lock lock(iLock);

    auto iter = iSubsribersByObject.find(aIOObjectId);

    if (iter == iSubsribersByObject.end())
    {
        iSubsribersByObject[aIOObjectId].Subscribe(aTaskId, std::move(aFn));
        OnAddObject(aIOObjectId);
    } else
    {
        iter->second.Subscribe(aTaskId, std::move(aFn));
    }
}

void    GpIOEventPoller::RemoveSubscription
(
    const GpIOObjectId  aIOObjectId,
    const GpTaskId      aTaskId
)
{
    std::scoped_lock lock(iLock);

    auto iter = iSubsribersByObject.find(aIOObjectId);

    if (iter == iSubsribersByObject.end())
    {
        return;
    }

    SubsribersEventChannelT& channel = iter->second;

    if (channel.Unsubscribe(aTaskId) == 0)
    {
        iSubsribersByObject.erase(aIOObjectId);
        OnRemoveObject(aIOObjectId);
    }
}

void    GpIOEventPoller::ProcessEvents
(
    const GpIOObjectId  aIOObjectId,
    GpIOEventsTypes     aEvents
)
{
    std::scoped_lock lock(iLock);

    auto iter = iSubsribersByObject.find(aIOObjectId);

    if (iter == iSubsribersByObject.end())
    {
        return;
    }

    SubsribersEventChannelT& channel = iter->second;
    channel.PushEvent(aEvents);

    if (   aEvents.Test(GpIOEventType::CLOSED)
        || aEvents.Test(GpIOEventType::ERROR)) [[unlikely]]
    {
        iSubsribersByObject.erase(aIOObjectId);
        OnRemoveObject(aIOObjectId);
    }
}

}//GPlatform

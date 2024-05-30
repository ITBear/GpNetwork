#include "GpIOEventPoller.hpp"

namespace GPlatform {

GpIOEventPoller::GpIOEventPoller (std::string aName) noexcept:
GpTaskFiber(std::move(aName))
{
}

GpIOEventPoller::~GpIOEventPoller (void) noexcept
{   
}

void    GpIOEventPoller::AddSubscription
(
    const GpSocketId                        aSocketId,
    const GpTaskId                          aTaskId,
    SubsribersEventChannelT::CallbackFnT&&  aFn
)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    auto iter = iSubsribersByIOObject.find(aSocketId);

    if (iter == std::end(iSubsribersByIOObject))
    {
        iSubsribersByIOObject[aSocketId].Subscribe(aTaskId, std::move(aFn));
        OnAddObject(aSocketId);
    } else
    {
        iter->second.Subscribe(aTaskId, std::move(aFn));
    }
}

void    GpIOEventPoller::RemoveSubscription
(
    const GpSocketId    aSocketId,
    const GpTaskId      aTaskId
)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

    auto iter = iSubsribersByIOObject.find(aSocketId);

    if (iter == std::end(iSubsribersByIOObject)) [[unlikely]]
    {
        return;
    }

    SubsribersEventChannelT& channel = iter->second;

    if (channel.Unsubscribe(aTaskId) == 0)
    {
        iSubsribersByIOObject.erase(aSocketId);
        OnRemoveObject(aSocketId);
    }
}

void    GpIOEventPoller::ProcessEvents
(
    const GpSocketId    aSocketId,
    GpIOEventsTypes     aEvents
)
{
    auto iter = iSubsribersByIOObject.find(aSocketId);

    if (iter == std::end(iSubsribersByIOObject))
    {
        return;
    }

    SubsribersEventChannelT& channel = iter->second;
    channel.PushEvent(SubsriberResValT{aSocketId, aEvents});

    if (   aEvents.Test(GpIOEventType::CLOSED)
        || aEvents.Test(GpIOEventType::ERROR_OCCURRED)) [[unlikely]]
    {
        iSubsribersByIOObject.erase(aSocketId);
        OnRemoveObject(aSocketId);
    }
}

}// namespace GPlatform

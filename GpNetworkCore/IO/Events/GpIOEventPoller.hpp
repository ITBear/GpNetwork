#pragma once

#include "GpIOEvent.hpp"
#include "../GpIOObjectId.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPoller: public GpLogTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpIOEventPoller)
    CLASS_DD(GpIOEventPoller)

    TAG_SET(THREAD_SAFE)

    using SubscribersT      = std::map<GpIOObjectId::IdT, GpEventSubscriber::SP, std::less<>>;
    using StartItcPromiseT  = GpItcPromise<size_t>;
    using StartItcFutureT   = GpItcFuture<size_t>;
    using StartItcResultT   = GpItcResult<size_t>;

protected:
    inline                      GpIOEventPoller         (std::u8string      aName,
                                                         StartItcPromiseT&& aStartPromise) noexcept;

public:
    virtual                     ~GpIOEventPoller        (void) noexcept override;

    void                        AddSubscriber           (GpEventSubscriber::SP  aSubscriber,
                                                         const GpIOObjectId     aIOObjectId);
    void                        RemoveSubscriber        (const GpIOObjectId     aIOObjectId);

protected:
    virtual void                OnStart                 (void) override = 0;
    virtual GpTaskDoRes         OnStep                  (EventOptRefT aEvent) override = 0;
    virtual void                OnStop                  (void) noexcept override;

    virtual void                OnAddSubscriber         (GpEventSubscriber::SP  aSubscriber,
                                                         const GpIOObjectId     aIOObjectId) = 0;
    virtual void                OnRemoveSubscriber      (const GpIOObjectId     aIOObjectId) = 0;

    inline void                 CompleteStartPromise    (StartItcResultT::SP aResult) noexcept {iStartPromise.Complete(std::move(aResult));}

private:
    mutable GpSpinlock          iSubscribersLock;
    SubscribersT                iSubscribers;
    StartItcPromiseT            iStartPromise;
};

GpIOEventPoller::GpIOEventPoller
(
    std::u8string       aName,
    StartItcPromiseT&&  aStartPromise
) noexcept:
GpLogTaskFiberBase(std::move(aName)),
iStartPromise(std::move(aStartPromise))
{
}

}//GPlatform

#pragma once

#include "GpIOEvent.hpp"
#include "../GpIOObjectId.hpp"

namespace GPlatform {

class GPNETWORK_API GpIOEventPoller: public GpTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS(GpIOEventPoller)
    CLASS_DECLARE_DEFAULTS(GpIOEventPoller)

    CLASS_TAG(THREAD_SAFE)

    using SubscribersT = GpMap<GpIOObjectId::RawT, GpEventSubscriber::SP>;

protected:  
                                    GpIOEventPoller     (std::string_view       aName,
                                                         GpTaskFiberBarrier::SP aStartBarrier);

public:
    virtual                         ~GpIOEventPoller    (void) noexcept override;

    void                            AddSubscriber       (GpEventSubscriber::SP  aSubscriber,
                                                         const GpIOObjectId     aIOObjectId);
    void                            RemoveSubscriber    (const GpIOObjectId     aIOObjectId);

protected:
    virtual void                    OnStart             (void) override;
    //virtual GpTask::ResT          OnStep              (EventOptRefT aEvent) override;
    virtual void                    OnStop              (void) noexcept override;

    virtual void                    OnAddSubscriber     (GpEventSubscriber::SP  aSubscriber,
                                                         const GpIOObjectId     aIOObjectId) = 0;
    virtual void                    OnRemoveSubscriber  (const GpIOObjectId     aIOObjectId) = 0;

    void                            ReleaseStartBarrier (void) noexcept;

private:
    mutable GpSpinlock              iSubscribersLock;
    SubscribersT                    iSubscribers;
    GpTaskFiberBarrier::SP          iStartBarrier;
};

}//GPlatform

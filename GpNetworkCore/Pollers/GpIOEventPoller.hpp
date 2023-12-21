#pragma once

#include "../GpIOObjectId.hpp"
#include "GpIOEventType.hpp"

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpUtils/EventBus/GpEventChannel.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPoller: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPoller)
    CLASS_DD(GpIOEventPoller)
    TAG_SET(THREAD_SAFE)

    using SubsribersEventChannelT   = GpEventChannel<GpTaskId, GpIOEventsTypes>;
    using SubsribersByObjectT       = std::unordered_map<GpIOObjectId, SubsribersEventChannelT>;

protected:
    inline                              GpIOEventPoller     (std::u8string aName) noexcept;

public:
    virtual                             ~GpIOEventPoller    (void) noexcept override;

    void                                AddSubscription     (const GpIOObjectId                     aIOObjectId,
                                                             const GpTaskId                         aTaskId,
                                                             SubsribersEventChannelT::CallbackFnT&& aFn);
    void                                RemoveSubscription  (const GpIOObjectId aIOObjectId,
                                                             const GpTaskId     aTaskId);

protected:
    void                                ProcessEvents       (const GpIOObjectId aIOObjectId,
                                                             GpIOEventsTypes    aEvents);

    virtual void                        OnStart             (void) override = 0;
    virtual GpTaskRunRes::EnumT         OnStep              (void) override = 0;
    virtual std::optional<GpException>  OnStop              (void) noexcept override = 0;

    virtual void                        OnAddObject         (const GpIOObjectId aIOObjectId) = 0;
    virtual void                        OnRemoveObject      (const GpIOObjectId aIOObjectId) = 0;

protected:
    mutable GpSpinLock                  iLock;

private:
    SubsribersByObjectT                 iSubsribersByObject;
};

GpIOEventPoller::GpIOEventPoller (std::u8string aName) noexcept:
GpTaskFiber(std::move(aName))
{
}

}//GPlatform

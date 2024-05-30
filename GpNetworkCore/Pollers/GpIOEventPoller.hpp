#pragma once

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

    using SubsriberResValT          = std::tuple<GpSocketId, GpIOEventsTypes>;
    using SubsribersEventChannelT   = GpEventChannel<GpTaskId, SubsriberResValT>;
    using SubsribersByObjectT       = std::unordered_map<GpSocketId, SubsribersEventChannelT>;

protected:
                                GpIOEventPoller     (std::string aName) noexcept;

public:
    virtual                     ~GpIOEventPoller    (void) noexcept override;

    void                        AddSubscription     (GpSocketId                             aSocketId,
                                                     GpTaskId                               aTaskId,
                                                     SubsribersEventChannelT::CallbackFnT&& aFn);
    void                        RemoveSubscription  (GpSocketId aSocketId,
                                                     GpTaskId   aTaskId);

protected:
    void                        ProcessEvents       (GpSocketId         aSocketId,
                                                     GpIOEventsTypes    aEvents) REQUIRES(iSpinLock);

    virtual void                OnStart             (void) override = 0;
    virtual GpTaskRunRes::EnumT OnStep              (void) override = 0;
    virtual GpException::C::Opt OnStop              (void) noexcept override = 0;

    virtual void                OnAddObject         (GpSocketId aSocketId) REQUIRES(iSpinLock) = 0;
    virtual void                OnRemoveObject      (GpSocketId aSocketId) REQUIRES(iSpinLock) = 0;

protected:
    mutable GpSpinLock          iSpinLock;

private:
    SubsribersByObjectT         iSubsribersByIOObject GUARDED_BY(iSpinLock);
};

}// namespace GPlatform

#pragma once

#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventType.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLock.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPoller: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPoller)
    CLASS_DD(GpIOEventPoller)
    TAG_SET(THREAD_SAFE)

    using SocketsByTaskT = ankerl::unordered_dense::map<GpSocketId, GpTask::WP>;

protected:
                                GpIOEventPoller     (std::string aName) noexcept;

public:
    virtual                     ~GpIOEventPoller    (void) noexcept override;

    void                        AddSubscription     (GpSocketId         aSocketId,
                                                     GpTask::WP         aTaskWP,
                                                     GpIOEventsTypes    aEventTypes);
    bool                        RemoveSubscription  (GpSocketId aSocketId);

protected:
    [[nodiscard]] bool          ProcessEvents       (GpSocketId         aSocketId,
                                                     GpIOEventsTypes    aEvents) REQUIRES(iSpinLock);

    virtual void                OnStart             (void) override;
    virtual GpTaskRunRes::EnumT OnStep              (void) override = 0;
    virtual void                OnStop              (ExceptionsT& aStopExceptionsOut) noexcept override;
    virtual void                OnStopException     (const GpException& aException) noexcept override = 0;

    virtual void                OnAddSocket         (GpSocketId         aSocketId,
                                                     GpIOEventsTypes    aEventTypes) REQUIRES(iSpinLock) = 0;
    virtual void                OnRemoveSocket      (GpSocketId aSocketId) REQUIRES(iSpinLock) = 0;

protected:
    mutable GpSpinLock<>    iSpinLock;

private:
    SocketsByTaskT          iSocketsByTask GUARDED_BY(iSpinLock);
};

}// namespace GPlatform

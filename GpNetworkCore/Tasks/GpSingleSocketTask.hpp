#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSocketTask.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLock.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSingleSocketTask: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSingleSocketTask)
    CLASS_DD(GpSingleSocketTask)
    TAG_SET(THREAD_SAFE)

    using IOEventsTypesT = std::atomic<GpIOEventsTypes::value_type>;

protected:
                                GpSingleSocketTask  (GpSocket::UP   aSocketUP) noexcept;
                                GpSingleSocketTask  (GpSocket::UP   aSocketUP,
                                                     std::string    aTaskName) noexcept;

public:
    virtual                     ~GpSingleSocketTask (void) noexcept override;

    virtual void                PushSocketEvents    (GpSocketId         aSocketId,
                                                     GpIOEventsTypes    aIoEvents) override final;

protected:
    GpSpinLock<>&               SpinLock            (void) const noexcept RETURN_CAPABILITY(iSpinLock) {return iSpinLock;}
    const GpSocket&             Socket              (void) const noexcept REQUIRES(iSpinLock) {return *iSocketUP;}
    GpSocket&                   Socket              (void) noexcept REQUIRES(iSpinLock) {return *iSocketUP;}

    virtual void                OnStart             (void) override = 0;
    virtual GpTaskRunRes::EnumT OnStep              (void) override;
    virtual void                OnStop              (ExceptionsT& aStopExceptionsOut) noexcept override = 0;
    virtual void                OnStopException     (const GpException& aException) noexcept override = 0;

    virtual void                ProcessSocketEvents (GpSocket&          aSocket,
                                                     GpIOEventsTypes    aIoEvents) REQUIRES(iSpinLock);

    virtual void                OnReadyToRead       (GpSocket& aSocket) REQUIRES(iSpinLock) = 0;
    virtual void                OnReadyToWrite      (GpSocket& aSocket) REQUIRES(iSpinLock) = 0;
    virtual void                OnClosed            (GpSocket& aSocket) REQUIRES(iSpinLock) = 0;
    virtual void                OnError             (GpSocket& aSocket) REQUIRES(iSpinLock) = 0;

private:
    mutable GpSpinLock<>    iSpinLock;
    IOEventsTypesT          iFiringIOEvents;
    GpSocket::UP            iSocketUP   GUARDED_BY(iSpinLock);
};

}// namespace GPlatform

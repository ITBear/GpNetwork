#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSocketTask.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpSocketTaskFactory.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTask.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpTcpAcceptServerTask : public GpSingleSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTcpAcceptServerTask)
    CLASS_DD(GpTcpAcceptServerTask)

public:
                    GpTcpAcceptServerTask   (GpSocketTCP::UP            aSocketUP,
                                             GpSocketAddr               aListenAddr,
                                             size_t                     aListenMaxQueueSize,
                                             GpSocketFlags              aAcceptSocketFlags,
                                             GpIOEventPollerIdx         aIOEventPollerIdx,
                                             GpSocketTaskFactory::UP    aSocketTaskFactoryUP) noexcept;
    virtual         ~GpTcpAcceptServerTask  (void) noexcept override;

protected:
    virtual void    OnStart                 (void) override;
    virtual void    OnStop                  (ExceptionsT& aStopExceptionsOut) noexcept override;
    virtual void    OnStopException         (const GpException& aException) noexcept override;

    virtual void    OnReadyToRead           (GpSocket& aSocket) REQUIRES(SpinLock()) override;
    virtual void    OnReadyToWrite          (GpSocket& aSocket) REQUIRES(SpinLock()) override;
    virtual void    OnClosed                (GpSocket& aSocket) REQUIRES(SpinLock()) override;
    virtual void    OnError                 (GpSocket& aSocket) REQUIRES(SpinLock()) override;

private:
    void            StartListen             (void);
    bool            VerifyBeforeListen      (void) const REQUIRES(SpinLock());

private:
    const GpSocketAddr              iListenAddr;
    const size_t                    iListenMaxQueueSize;
    const GpSocketFlags             iAcceptSocketFlags;
    const GpIOEventPollerIdx        iIOEventPollerIdx;
    const GpSocketTaskFactory::UP   iSocketTaskFactoryUP;
};

}// namespace GPlatform

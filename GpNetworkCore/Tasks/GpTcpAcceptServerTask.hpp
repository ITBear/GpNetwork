#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSocketsTask.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpTcpServerTaskFactory.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpTcpAcceptServerTask final: public GpSocketsTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTcpAcceptServerTask)
    CLASS_DD(GpTcpAcceptServerTask)

public:
                                        GpTcpAcceptServerTask   (GpSocketAddr               aListenAddr,
                                                                 GpSocketFlags              aListenSocketFlags,
                                                                 size_t                     aListenMaxQueueSize,
                                                                 GpSocketFlags              aAcceptSocketFlags,
                                                                 GpIOEventPollerIdx         aIOEventPollerIdx,                                                               
                                                                 GpTcpServerTaskFactory::SP aServerTaskFactory) noexcept;
    virtual                             ~GpTcpAcceptServerTask  (void) noexcept override final;

protected:
    virtual void                        OnStart                 (void) override final;
    virtual void                        OnStop                  (StopExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void                        OnStopException         (const GpException& aException) noexcept override final;

    virtual void                        OnReadyToRead           (GpSocket& aSocket) override final;
    virtual void                        OnReadyToWrite          (GpSocket& aSocket) override final;
    virtual void                        OnClosed                (GpSocket& aSocket) override final;
    virtual void                        OnError                 (GpSocket& aSocket) override final;
    virtual void                        ProcessOtherMessages    (GpAny& aMessage) override final;
    virtual GpSocket::SP                FindSocket              (GpSocketId aSocketId) override final;

private:
    const GpSocketAddr                  iListenAddr;
    const GpSocketFlags                 iListenSocketFlags;
    const size_t                        iListenMaxQueueSize;
    const GpSocketFlags                 iAcceptSocketFlags;
    const GpIOEventPollerIdx            iIOEventPollerIdx;
    const GpTcpServerTaskFactory::CSP   iServerTaskFactory;

    GpSocketTCP::SP                     iListenSocketSP;
};

}// namespace GPlatform

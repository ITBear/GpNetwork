#pragma once

#include "GpSingleSocketTask.hpp"
#include "GpSingleSocketTaskFactory.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpTcpAcceptServerTask final: public GpSingleSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTcpAcceptServerTask)
    CLASS_DD(GpTcpAcceptServerTask)

public:
                                    GpTcpAcceptServerTask   (GpSocketAddr                   aSocketAddr,
                                                             size_t                         aSocketMaxQueueSize,
                                                             GpSocketFlags                  aListenSocketFlags,
                                                             GpSocketFlags                  aAcceptSocketFlags,
                                                             GpIOEventPollerIdx             aIOEventPollerIdx,
                                                             GpSingleSocketTaskFactory::SP  aTaskFactory) noexcept;
    virtual                         ~GpTcpAcceptServerTask  (void) noexcept override final;

protected:
    virtual void                    OnReadyToRead           (GpSocket& aSocket) override final;
    virtual void                    OnReadyToWrite          (GpSocket& aSocket) override final;
    virtual void                    OnClosed                (GpSocket& aSocket) override final;
    virtual void                    OnError                 (GpSocket& aSocket) override final;
    virtual void                    ProcessOtherMessages    (GpAny& aMessage) override final;

private:
    const GpSocketFlags             iAcceptSocketFlags;
    GpSingleSocketTaskFactory::SP   iTaskFactory;
};

}// namespace GPlatform

#pragma once

#include "../Pollers/GpIOEventPoller.hpp"
#include "GpSocketTask.hpp"
#include "GpSocketTaskFactory.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpTcpAcceptServerTask final: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTcpAcceptServerTask)
    CLASS_DD(GpTcpAcceptServerTask)

public:
                                        GpTcpAcceptServerTask   (GpSocketAddr               aSocketAddr,
                                                                 const size_t               aSocketMaxQueueSize,
                                                                 const GpSocketFlags        aListenSocketFlags,
                                                                 const GpSocketFlags        aAcceptSocketFlags,
                                                                 std::u8string              aEventPollerName,
                                                                 GpSocketTaskFactory::SP    aTaskFactory) noexcept;
    virtual                             ~GpTcpAcceptServerTask  (void) noexcept override final;

protected:
    virtual GpTaskRunRes::EnumT         OnReadyToRead           (GpSocket& aSocket) override final;
    virtual GpTaskRunRes::EnumT         OnReadyToWrite          (GpSocket& aSocket) override final;
    virtual void                        OnClosed                (GpSocket& aSocket) override final;
    virtual void                        OnError                 (GpSocket& aSocket) override final;

private:
    const GpSocketFlags                 iAcceptSocketFlags;
    GpSocketTaskFactory::SP             iTaskFactory;
    const std::u8string                 iEventPollerName;
    GpIOEventPoller*                    iEventPoller = nullptr;
};

}//namespace GPlatform

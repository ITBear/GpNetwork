#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../GpNetworkCore/Tasks/GpSocketTask.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestSocketTask final: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpRequestSocketTask)
    CLASS_DD(GpHttpRequestSocketTask)

public:
    inline                          GpHttpRequestSocketTask     (GpSocket::SP   aSocket) noexcept;
    virtual                         ~GpHttpRequestSocketTask    (void) noexcept override final;

    virtual GpTaskRunRes::EnumT     OnReadyToRead               (GpSocket& aSocket) override final;
    virtual GpTaskRunRes::EnumT     OnReadyToWrite              (GpSocket& aSocket) override final;
    virtual void                    OnClosed                    (GpSocket& aSocket) override final;
    virtual void                    OnError                     (GpSocket& aSocket) override final;

private:
};

GpHttpRequestSocketTask::GpHttpRequestSocketTask
(
    GpSocket::SP    aSocket
) noexcept:
GpSocketTask(std::move(aSocket))
{
}

}//namespace GPlatform

#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../GpNetworkCore/Tasks/GpSingleSocketTask.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestSocketTask final: public GpSingleSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpRequestSocketTask)
    CLASS_DD(GpHttpRequestSocketTask)

public:
    inline                          GpHttpRequestSocketTask     (GpSocket::SP   aSocket) noexcept;
    virtual                         ~GpHttpRequestSocketTask    (void) noexcept override final;

    virtual void                    OnReadyToRead               (GpSocket& aSocket) override final;
    virtual void                    OnReadyToWrite              (GpSocket& aSocket) override final;
    virtual void                    OnClosed                    (GpSocket& aSocket) override final;
    virtual void                    OnError                     (GpSocket& aSocket) override final;

private:
};

GpHttpRequestSocketTask::GpHttpRequestSocketTask
(
    GpSocket::SP    aSocket
) noexcept:
GpSingleSocketTask(std::move(aSocket))
{
}

}// namespace GPlatform

#pragma once

#include "GpHttpRequestHandler.hpp"
#include "../../../../GpLog/GpLogCore/Tasks/GpLogTaskFiberBase.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestTask final: public GpLogTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpRequestTask)
    CLASS_DD(GpHttpRequestTask)

public:
    inline                      GpHttpRequestTask   (std::u8string              aName,
                                                     GpHttpRequest::SP          aHttpRequest,
                                                     GpHttpRequestHandler::SP   aHttpRequestHandler,
                                                     GpEventSubscriber&         aHttpRequestDoneSbr) noexcept;
    virtual                     ~GpHttpRequestTask  (void) noexcept override final {}

    virtual void                OnStart             (void) override final;
    virtual GpTaskDoRes         OnStep              (EventOptRefT aEvent) override final;
    virtual void                OnStop              (void) noexcept override final;

private:
    GpHttpRequest::SP           iHttpRequest;
    GpHttpRequestHandler::SP    iHttpRequestHandler;
    GpEventSubscriber&          iHttpRequestDoneSbr;
};

GpHttpRequestTask::GpHttpRequestTask
(
    std::u8string               aName,
    GpHttpRequest::SP           aHttpRequest,
    GpHttpRequestHandler::SP    aHttpRequestHandler,
    GpEventSubscriber&          aHttpRequestDoneSbr
) noexcept:
GpLogTaskFiberBase(std::move(aName)),
iHttpRequest(std::move(aHttpRequest)),
iHttpRequestHandler(std::move(aHttpRequestHandler)),
iHttpRequestDoneSbr(aHttpRequestDoneSbr)
{
}

}//namespace GPlatform

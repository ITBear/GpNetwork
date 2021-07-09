#pragma once

#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequestTask final: public GpTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpRequestTask)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestTask)

public:
                                    GpHttpRequestTask   (std::string_view           aName,
                                                         GpHttpRequest::CSP         aHttpRequest,
                                                         GpHttpRequestHandler::SP   aHttpRequestHandler,
                                                         GpEventSubscriber::SP      aHttpRequestDoneSbr);
    virtual                         ~GpHttpRequestTask  (void) noexcept override final;

    virtual void                    OnStart             (void) override final;
    virtual GpTask::ResT            OnStep              (EventOptRefT aEvent) override final;
    virtual void                    OnStop              (void) noexcept override final;

private:
    GpHttpRequest::CSP              iHttpRequest;
    GpHttpRequestHandler::SP        iHttpRequestHandler;
    GpEventSubscriber::SP           iHttpRequestDoneSbr;
};

}//namespace GPlatform

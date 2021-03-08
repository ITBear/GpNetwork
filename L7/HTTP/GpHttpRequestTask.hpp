#pragma once

#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequestTask final: public GpTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS(GpHttpRequestTask)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestTask)

public:
                                    GpHttpRequestTask   (GpHttpRequest::CSP         aHttpRequest,
                                                         GpHttpRequestHandler::SP   aHttpRequestHandler,
                                                         GpEventSubscriber::SP      aHttpRequestDoneSbr) noexcept;
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
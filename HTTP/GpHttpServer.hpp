#pragma once

#include "GpHttpRequestHandlerFactory.hpp"
#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpServer: public GpBaseTaskFiber
{
public:
    CLASS_REMOVE_CTRS(GpHttpServer)
    CLASS_DECLARE_DEFAULTS(GpHttpServer)

public:
                                        GpHttpServer            (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept;
    virtual                             ~GpHttpServer           (void) noexcept override;

    GpHttpRequestHandlerFactory::SP     RequestHandlerFactory   (void) {return iRequestHandlerFactory;}

protected:
    GpHttpRequestHandler::SP            NewRequestHandler       (void) const;

    virtual void                        OnStart                 (void) override;
    virtual void                        OnStep                  (EventOptRefT aEvent) override;
    virtual void                        OnStop                  (void) noexcept override;

private:
    GpHttpRequestHandlerFactory::SP     iRequestHandlerFactory;
};

}//namespace GPlatform

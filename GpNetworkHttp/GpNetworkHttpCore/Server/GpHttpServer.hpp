#pragma once

#include "../RqRs/GpHttpRqRs.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServer: public GpLogTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServer)
    CLASS_DD(GpHttpServer)

protected:
    inline                              GpHttpServer            (std::string                        aName,
                                                                 GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;

public:
    virtual                             ~GpHttpServer           (void) noexcept override;

protected:
    GpHttpRequestHandlerFactory::SP     RequestHandlerFactory   (void) {return iRequestHandlerFactory;}

    virtual void                        OnStart                 (void) override = 0;
    virtual GpTaskDoRes                 OnStep                  (EventOptRefT aEvent) override = 0;
    virtual void                        OnStop                  (void) noexcept override = 0;

private:
    GpHttpRequestHandlerFactory::SP     iRequestHandlerFactory;
};

GpHttpServer::GpHttpServer
(
    std::string                     aName,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
) noexcept:
GpLogTaskFiberBase(std::move(aName)),
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

}//namespace GPlatform

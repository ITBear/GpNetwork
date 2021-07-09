#pragma once

#include "GpHttpRequestHandlerFactory.hpp"
#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpServer: public GpTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServer)
    CLASS_DECLARE_DEFAULTS(GpHttpServer)

public:
                                        GpHttpServer            (std::string_view                   aName,
                                                                 GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory);
    virtual                             ~GpHttpServer           (void) noexcept override;

protected:
    GpHttpRequestHandlerFactory::SP     RequestHandlerFactory   (void) {return iRequestHandlerFactory;}

private:
    GpHttpRequestHandlerFactory::SP     iRequestHandlerFactory;
};

}//namespace GPlatform

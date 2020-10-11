#pragma once

#include "../GpHttpRequestHandlerFactory.hpp"

GP_WARNING_PUSH()
GP_WARNING_DISABLE(deprecated)
GP_WARNING_DISABLE(shadow)

#include <pistache/endpoint.h>
#include <pistache/router.h>

GP_WARNING_POP()

namespace GPlatform {

class GpHttpRouteHandlerPistache
{
public:
    CLASS_DECLARE_DEFAULTS(GpHttpRouteHandlerPistache)

public:
                                    GpHttpRouteHandlerPistache  (Pistache::Rest::Router&            aHttpRouter,
                                                                 GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory);
                                    ~GpHttpRouteHandlerPistache (void) noexcept;

        void                        Handle                      (const Pistache::Rest::Request& aRequest,
                                                                 Pistache::Http::ResponseWriter aResponseWriter);

private:
    GpHttpRequestHandlerFactory::SP iRequestHandlerFactory;
};

}//namespace GPlatform

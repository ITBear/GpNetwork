#pragma once

#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestRouter: public GpHttpRequestHandler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestRouter)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestRouter)

protected:
                                    GpHttpRequestRouter     (void) noexcept {}

public:
    virtual                         ~GpHttpRequestRouter    (void) noexcept override {}

    //virtual GpHttpResponse::SP    OnRequest               (const GpHttpRequest& aRequest) = 0;
};

}//namespace GPlatform

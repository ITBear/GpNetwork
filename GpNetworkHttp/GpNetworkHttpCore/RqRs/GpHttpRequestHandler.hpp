#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestHandler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestHandler)
    CLASS_DD(GpHttpRequestHandler)

    TAG_SET(THREAD_SAFE)

protected:
                                GpHttpRequestHandler    (void) noexcept {}

public:
    virtual                     ~GpHttpRequestHandler   (void) noexcept {}

    GpHttpResponse::SP          ProcessRequest          (GpHttpRequest& aRequest);

protected:
    virtual GpHttpResponse::SP  OnRequest               (GpHttpRequest& aRequest) = 0;
};

}//namespace GPlatform

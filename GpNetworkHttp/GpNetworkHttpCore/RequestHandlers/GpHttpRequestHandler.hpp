#pragma once

#include "../RqRs/GpHttpRequest.hpp"
#include "../RqRs/GpHttpResponse.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestHandler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestHandler)
    CLASS_DD(GpHttpRequestHandler)

protected:
                                GpHttpRequestHandler    (void) noexcept {}

public:
    virtual                     ~GpHttpRequestHandler   (void) noexcept {}

    GpHttpResponse::SP          ProcessRequest          (GpHttpRequest::SP aRequest);

protected:
    virtual GpHttpResponse::SP  OnRequest               (GpHttpRequest& aRequest) = 0;
};

}// namespace GPlatform

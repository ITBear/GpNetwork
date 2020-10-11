#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequestHandler
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpRequestHandler)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestHandler)

protected:
                                GpHttpRequestHandler    (void) noexcept {}

public:
    virtual                     ~GpHttpRequestHandler   (void) noexcept {}

    virtual GpHttpResponse::SP  OnRequest               (GpHttpRequest::SP aRequest) = 0;
};

}//namespace GPlatform

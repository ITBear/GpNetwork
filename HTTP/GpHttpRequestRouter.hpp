#pragma once

#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequestRouter: public GpHttpRequestHandler
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpRequestRouter)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestRouter)

protected:
                                    GpHttpRequestRouter     (void) noexcept {}

public:
    virtual                         ~GpHttpRequestRouter    (void) noexcept override {}

    //virtual GpHttpResponse::SP    OnRequest               (const GpHttpRequest& aRequest) = 0;
};

}//namespace GPlatform

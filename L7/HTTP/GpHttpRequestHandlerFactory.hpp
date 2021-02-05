#pragma once

#include "../../GpNetwork_global.hpp"

namespace GPlatform {

class GpHttpRequestHandler;

class GPNETWORK_API GpHttpRequestHandlerFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpRequestHandlerFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestHandlerFactory)

protected:
                                        GpHttpRequestHandlerFactory     (void) noexcept {}

public:
    virtual                             ~GpHttpRequestHandlerFactory    (void) noexcept {}

    virtual GpSP<GpHttpRequestHandler>  NewInstance                     (void) const = 0;
};

}//namespace GPlatform

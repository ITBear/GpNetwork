#pragma once

#include "../GpNetwork_global.hpp"

namespace GPlatform {

class GpHttpServer;
class GpHttpRequestHandlerFactory;

class GPNETWORK_API GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpServerFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerFactory)

protected:
                                GpHttpServerFactory     (void) noexcept {}

public:
    virtual                     ~GpHttpServerFactory    (void) noexcept {}

    virtual GpSP<GpHttpServer>  NewInstance             (GpSP<GpHttpRequestHandlerFactory >aRequestHandlerFactory) const = 0;
};

}//namespace GPlatform

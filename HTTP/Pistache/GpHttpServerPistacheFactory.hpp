#pragma once

#include "../GpHttpServerFactory.hpp"
#include "../GpHttpRequestHandlerFactory.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpServerPistacheFactory final: public GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS(GpHttpServerPistacheFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerPistacheFactory)

public:
                                GpHttpServerPistacheFactory     (const GpSocketAddr&                aListenSocketAddr,
                                                                 GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;
    virtual                     ~GpHttpServerPistacheFactory    (void) noexcept override final;

    virtual GpSP<GpHttpServer>  NewInstance                     (void) const override final;
};

}//namespace GPlatform

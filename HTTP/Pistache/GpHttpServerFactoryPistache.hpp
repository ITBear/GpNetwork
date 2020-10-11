#pragma once

#include "../GpHttpServerFactory.hpp"
#include "../GpHttpRequestHandlerFactory.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpServerFactoryPistache final: public GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpServerFactoryPistache)
    CLASS_DECLARE_DEFAULTS(GpHttpServerFactoryPistache)

public:
                                GpHttpServerFactoryPistache     (void) noexcept;
    virtual                     ~GpHttpServerFactoryPistache    (void) noexcept override final;

    virtual GpSP<GpHttpServer>  NewInstance                     (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) const override final;
};

}//namespace GPlatform

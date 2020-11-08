#pragma once

#include "../GpHttpServerFactory.hpp"
#include "../GpHttpRequestHandlerFactory.hpp"
#include "../../IO/Events/GpIOEventPollerFactory.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpServerNodeFactory final: public GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS(GpHttpServerNodeFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerNodeFactory)

public:
                                GpHttpServerNodeFactory     (const GpSocketAddr&                aListenSocketAddr,
                                                             GpIOEventPollerFactory::SP         aEventPollerFactory,
                                                             GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;
    virtual                     ~GpHttpServerNodeFactory    (void) noexcept override final;

    virtual GpSP<GpHttpServer>  NewInstance                 (void) const override final;

private:
    GpIOEventPollerFactory::SP  iEventPollerFactory;
};

}//namespace GPlatform

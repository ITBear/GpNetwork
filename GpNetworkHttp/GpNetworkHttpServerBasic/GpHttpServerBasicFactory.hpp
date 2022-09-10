#pragma once

#include "GpNetworkHttpServerBasic_global.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_SERVER_BASIC_API GpHttpServerBasicFactory final: public GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerBasicFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerBasicFactory)

public:
    inline                      GpHttpServerBasicFactory    (const GpSocketAddr&                aListenSocketAddr,
                                                             GpIOEventPoller&                   aEventPoller,
                                                             GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;
    virtual                     ~GpHttpServerBasicFactory   (void) noexcept override final;

    virtual GpSP<GpHttpServer>  NewInstance                 (std::string aName) const override final;

private:
    GpIOEventPoller&            iEventPoller;
};

GpHttpServerBasicFactory::GpHttpServerBasicFactory
(
    const GpSocketAddr&             aListenSocketAddr,
    GpIOEventPoller&                aEventPoller,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
) noexcept:
GpHttpServerFactory
(
    aListenSocketAddr,
    std::move(aRequestHandlerFactory)
),
iEventPoller(aEventPoller)
{
}

}//namespace GPlatform

#pragma once

#include "GpNetworkHttpServerBasic_global.hpp"
#include "../GpNetworkHttpCore/Server/GpHttpServerFactory.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_SERVER_BASIC_API GpHttpServerBasicFactory final: public GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerBasicFactory)
    CLASS_DD(GpHttpServerBasicFactory)

public:
    inline                      GpHttpServerBasicFactory    (const GpSocketAddr&                aListenSocketAddr,
                                                             GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;
    virtual                     ~GpHttpServerBasicFactory   (void) noexcept override final;

    virtual GpSP<GpHttpServer>  NewInstance                 (std::u8string          aName,
                                                             GpIOEventPoller::SP    aEventPoller) const override final;

private:
    GpIOEventPoller::SP         iEventPoller;
};

GpHttpServerBasicFactory::GpHttpServerBasicFactory
(
    const GpSocketAddr&             aListenSocketAddr,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
) noexcept:
GpHttpServerFactory
(
    aListenSocketAddr,
    std::move(aRequestHandlerFactory)
)
{
}

}//namespace GPlatform

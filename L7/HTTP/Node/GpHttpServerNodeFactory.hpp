#pragma once

#include "../GpHttpServerFactory.hpp"
#include "../GpHttpRequestHandlerFactory.hpp"
#include "../../../IO/Events/GpIOEventPoller.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpServerNodeFactory final: public GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS(GpHttpServerNodeFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerNodeFactory)

public:
                                GpHttpServerNodeFactory     (const GpSocketAddr&                aListenSocketAddr,
                                                             GpIOEventPoller::WP                aEventPoller,
                                                             GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;
    virtual                     ~GpHttpServerNodeFactory    (void) noexcept override final;

    virtual GpSP<GpHttpServer>  NewInstance                 (std::string_view aName) const override final;

private:
    GpIOEventPoller::WP         iEventPoller;
};

}//namespace GPlatform

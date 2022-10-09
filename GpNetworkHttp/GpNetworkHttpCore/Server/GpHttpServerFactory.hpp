#pragma once

#include "../RqRs/GpHttpRqRs.hpp"

namespace GPlatform {

class GpHttpServer;
class GpHttpRequestHandlerFactory;

class GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerFactory)
    CLASS_DD(GpHttpServerFactory)

protected:
    inline                              GpHttpServerFactory     (const GpSocketAddr&                aListenSocketAddr,
                                                                 GpSP<GpHttpRequestHandlerFactory>  aRequestHandlerFactory) noexcept;

public:
    virtual                             ~GpHttpServerFactory    (void) noexcept {}

    virtual GpSP<GpHttpServer>          NewInstance             (std::string            aName,
                                                                 GpIOEventPoller::SP    aEventPoller) const = 0;

protected:
    const GpSocketAddr&                 ListenSocketAddr        (void) const noexcept {return iListenSocketAddr;}
    GpSP<GpHttpRequestHandlerFactory>   RequestHandlerFactory   (void) const {return iRequestHandlerFactory;}

private:
    const GpSocketAddr                  iListenSocketAddr;
    GpSP<GpHttpRequestHandlerFactory>   iRequestHandlerFactory;
};

GpHttpServerFactory::GpHttpServerFactory
(
    const GpSocketAddr&             aListenSocketAddr,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
) noexcept:
iListenSocketAddr(aListenSocketAddr),
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

}//namespace GPlatform

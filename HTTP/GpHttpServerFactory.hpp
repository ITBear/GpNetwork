#pragma once

#include "../GpNetwork_global.hpp"
#include "../IO/Sockets/GpSocketAddr.hpp"

namespace GPlatform {

class GpHttpServer;
class GpHttpRequestHandlerFactory;

class GPNETWORK_API GpHttpServerFactory
{
public:
    CLASS_REMOVE_CTRS(GpHttpServerFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerFactory)

protected:
                                        GpHttpServerFactory     (const GpSocketAddr&                aListenSocketAddr,
                                                                 GpSP<GpHttpRequestHandlerFactory>  aRequestHandlerFactory) noexcept;

public:
    virtual                             ~GpHttpServerFactory    (void) noexcept;

    virtual GpSP<GpHttpServer>          NewInstance             (void) const = 0;

protected:
    const GpSocketAddr&                 ListenSocketAddr        (void) const noexcept {return iListenSocketAddr;}
    GpSP<GpHttpRequestHandlerFactory>   RequestHandlerFactory   (void) const {return iRequestHandlerFactory;}

private:
    const GpSocketAddr                  iListenSocketAddr;
    GpSP<GpHttpRequestHandlerFactory>   iRequestHandlerFactory;
};

}//namespace GPlatform

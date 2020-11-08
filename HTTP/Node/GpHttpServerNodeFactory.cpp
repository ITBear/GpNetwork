#include "GpHttpServerNodeFactory.hpp"
#include "GpHttpServerNode.hpp"

namespace GPlatform {

GpHttpServerNodeFactory::GpHttpServerNodeFactory (const GpSocketAddr&               aListenSocketAddr,
                                                  GpIOEventPollerFactory::SP        aEventPollerFactory,
                                                  GpHttpRequestHandlerFactory::SP   aRequestHandlerFactory) noexcept:
GpHttpServerFactory(aListenSocketAddr, std::move(aRequestHandlerFactory)),
iEventPollerFactory(std::move(aEventPollerFactory))
{
}

GpHttpServerNodeFactory::~GpHttpServerNodeFactory (void) noexcept
{
}

GpHttpServer::SP    GpHttpServerNodeFactory::NewInstance (void) const
{
    return MakeSP<GpHttpServerNode>(ListenSocketAddr(),
                                    iEventPollerFactory,
                                    RequestHandlerFactory());
}

}//namespace GPlatform

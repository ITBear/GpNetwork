#include "GpHttpServerPistacheFactory.hpp"
#include "GpHttpServerPistache.hpp"

namespace GPlatform {

GpHttpServerPistacheFactory::GpHttpServerPistacheFactory (const GpSocketAddr&               aListenSocketAddr,
                                                          GpHttpRequestHandlerFactory::SP   aRequestHandlerFactory) noexcept:
GpHttpServerFactory(aListenSocketAddr,
                    std::move(aRequestHandlerFactory))
{
}

GpHttpServerPistacheFactory::~GpHttpServerPistacheFactory (void) noexcept
{
}

GpHttpServer::SP    GpHttpServerPistacheFactory::NewInstance (void) const
{
    return MakeSP<GpHttpServerPistache>(RequestHandlerFactory());
}

}//namespace GPlatform

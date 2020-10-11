#include "GpHttpServerFactoryPistache.hpp"
#include "GpHttpServerPistache.hpp"

namespace GPlatform {

GpHttpServerFactoryPistache::GpHttpServerFactoryPistache (void) noexcept
{
}

GpHttpServerFactoryPistache::~GpHttpServerFactoryPistache (void) noexcept
{
}

GpHttpServer::SP    GpHttpServerFactoryPistache::NewInstance (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) const
{
    return MakeSP<GpHttpServerPistache>(aRequestHandlerFactory);
}

}//namespace GPlatform

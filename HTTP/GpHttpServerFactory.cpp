#include "GpHttpServerFactory.hpp"
#include "GpHttpRequestHandlerFactory.hpp"

namespace GPlatform {

GpHttpServerFactory::GpHttpServerFactory (const GpSocketAddr&               aListenSocketAddr,
                                          GpHttpRequestHandlerFactory::SP   aRequestHandlerFactory) noexcept:
iListenSocketAddr(aListenSocketAddr),
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

GpHttpServerFactory::~GpHttpServerFactory (void) noexcept
{
}

}//namespace GPlatform

#include "GpHttpServerSocketTaskFactory.hpp"

namespace GPlatform {

GpHttpServerSocketTaskFactory::GpHttpServerSocketTaskFactory (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

GpHttpServerSocketTaskFactory::~GpHttpServerSocketTaskFactory (void) noexcept
{
}

}//namespace GPlatform

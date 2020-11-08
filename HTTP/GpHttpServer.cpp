#include "GpHttpServer.hpp"

namespace GPlatform {

GpHttpServer::GpHttpServer (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

GpHttpServer::~GpHttpServer (void) noexcept
{
}

}//namespace GPlatform

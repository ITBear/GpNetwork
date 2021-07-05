#include "GpHttpServer.hpp"

namespace GPlatform {

GpHttpServer::GpHttpServer
(
    std::string_view                aName,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
):
GpTaskFiberBase(aName),
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

GpHttpServer::~GpHttpServer (void) noexcept
{
}

}//namespace GPlatform

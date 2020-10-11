#include "GpHttpServer.hpp"

namespace GPlatform {

GpHttpServer::GpHttpServer (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

GpHttpServer::~GpHttpServer (void) noexcept
{
}

GpHttpRequestHandler::SP    GpHttpServer::NewRequestHandler (void) const
{
    return iRequestHandlerFactory.VC().NewInstance();
}

void    GpHttpServer::OnStart (void)
{
}

void    GpHttpServer::OnStep (EventOptRefT /*aEvent*/)
{
}

void    GpHttpServer::OnStop (void) noexcept
{
}

}//namespace GPlatform

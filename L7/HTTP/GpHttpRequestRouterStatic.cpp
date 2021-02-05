#include "GpHttpRequestRouterStatic.hpp"

namespace GPlatform {

GpHttpRequestRouterStatic::GpHttpRequestRouterStatic (GpHttpRequestRouteTable::CSP aRouteTable) noexcept:
iRouteTable(std::move(aRouteTable))
{
}

GpHttpRequestRouterStatic::~GpHttpRequestRouterStatic (void) noexcept
{
}

GpHttpResponse::SP  GpHttpRequestRouterStatic::OnRequest (const GpHttpRequest& aRequest) const
{
    const GpHttpRequestRouteTable&  routeTable      = iRouteTable.VCn();
    GpHttpRequestHandler::SP        requestHandler  = routeTable.Handler(aRequest.url);

    return requestHandler.Vn().ProcessRequest(aRequest);
}

}//namespace GPlatform

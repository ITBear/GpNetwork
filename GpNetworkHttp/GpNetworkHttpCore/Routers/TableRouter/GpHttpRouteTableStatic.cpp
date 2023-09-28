#include "GpHttpRouteTableStatic.hpp"
#include "../../Exceptions/GpHttpException.hpp"
#include <mutex>
#include <shared_mutex>

namespace GPlatform {

GpHttpRouteTableStatic::GpHttpRouteTableStatic (void) noexcept
{
}

GpHttpRouteTableStatic::~GpHttpRouteTableStatic (void) noexcept
{
}

GpHttpRequestHandler::SP    GpHttpRouteTableStatic::FindHandler (const GpHttpRequestNoBodyDesc& aHttpRqNoBody) const
{
    std::shared_lock lock(iLock);

    std::u8string_view urlHost  = aHttpRqNoBody.url.Authority().Host();
    std::u8string_view urlPath  = aHttpRqNoBody.url.Path();

    // Try to find host
    const auto hostIter = iRouteRable.find(urlHost);

    THROW_COND_HTTP
    (
        hostIter != iRouteRable.end(),
        GpHttpResponseCode::NOT_FOUND_404,
        [urlHost](){return u8"Handler not found for URL host '"_sv + urlHost + u8"'"_sv;}
    );

    // Try to find path
    const auto& pathMap = hostIter->second;
    const auto pathIter = pathMap.find(urlPath);

    THROW_COND_HTTP
    (
        pathIter != pathMap.end(),
        GpHttpResponseCode::NOT_FOUND_404,
        [aHttpRqNoBody](){return u8"Handler not found for URL '"_sv + aHttpRqNoBody.url.SchemeHostPortPath() + u8"'"_sv;}
    );

    return pathIter->second->NewInstance();
}

void    GpHttpRouteTableStatic::RegisterDefaultHandler (GpHttpRequestHandlerFactory::SP aHandlerFactory)
{
    std::scoped_lock lock(iLock);

    iDefaultHandlerFactory = std::move(aHandlerFactory);
}

void    GpHttpRouteTableStatic::RegisterPathHandler
(
    std::u8string_view              aHost,
    std::u8string_view              aPath,
    GpHttpRequestHandlerFactory::SP aHandlerFactory
)
{
    std::scoped_lock lock(iLock);

    iRouteRable[std::u8string(aHost)][std::u8string(aPath)] = std::move(aHandlerFactory);
}

/*void  GpHttpRouteTableStatic::RegisterPath
(
    std::u8string_view              aPath,
    GpHttpRequestHandlerFactory::SP aHandlerFactory
)
{
    iHandlersCatalog.Set(aPath, std::move(aHandlerFactory));
}*/

}//namespace GPlatform

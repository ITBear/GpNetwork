#include "GpHttpRouteTableStatic.hpp"
#include "../../Exceptions/GpHttpException.hpp"

#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>

namespace GPlatform {

GpHttpRouteTableStatic::GpHttpRouteTableStatic (void) noexcept
{
}

GpHttpRouteTableStatic::~GpHttpRouteTableStatic (void) noexcept
{
}

GpHttpRequestHandler::SP    GpHttpRouteTableStatic::FindHandler (const GpHttpRequestNoBodyDesc& aHttpRqNoBody) const
{
    GpSharedLock<GpSpinLockRW> sharedLock(iSpinLockRW);

    std::string_view urlHost = aHttpRqNoBody.url.Authority().Host();
    std::string_view urlPath = aHttpRqNoBody.url.Path();

    // Try to find host
    const auto hostIter = iRouteRable.find(urlHost);

    THROW_COND_HTTP
    (
        hostIter != std::end(iRouteRable),
        GpHttpResponseCode::NOT_FOUND_404,
        [urlHost]()
        {
            return fmt::format
            (
                "Handler not found for host '{}'",
                urlHost
            );
        }
    );

    // Try to find path
    const auto& pathMap = hostIter->second;
    const auto pathIter = pathMap.find(urlPath);

    THROW_COND_HTTP
    (
        pathIter != std::end(pathMap),
        GpHttpResponseCode::NOT_FOUND_404,
        [aHttpRqNoBody]()
        {
            return fmt::format
            (
                "Handler not found for URL '{}'",
                aHttpRqNoBody.url.SchemeHostPortPath()
            );
        }
    );

    return pathIter->second->NewInstance();
}

void    GpHttpRouteTableStatic::RegisterDefaultHandler (GpHttpRequestHandlerFactory::SP aHandlerFactory)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    iDefaultHandlerFactory = std::move(aHandlerFactory);
}

void    GpHttpRouteTableStatic::RegisterPathHandler
(
    std::string                     aHost,
    std::string                     aPath,
    GpHttpRequestHandlerFactory::SP aHandlerFactory
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    iRouteRable[std::move(aHost)][std::move(aPath)] = std::move(aHandlerFactory);
}

}// namespace GPlatform

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

    const GpUrlAuthority&   urlAuthority        = aHttpRqNoBody.url.Authority();
    const std::string_view  urlPath             = aHttpRqNoBody.url.Path();
    const std::string_view  urlHost             = urlAuthority.Host();
    const u_int_16          urlPort             = urlAuthority.Port();
    const std::string       routePathHostPort   = fmt::format("{}:{}", urlHost, urlPort);

    // Try to find host
    const auto hostIter = iRouteRable.find(routePathHostPort);

    // Check if default handler set
    if (hostIter == std::end(iRouteRable)) [[unlikely]]
    {
        if (iDefaultHandlerFactory.IsNotNULL()) [[likely]]
        {
            return iDefaultHandlerFactory.Vn().NewInstance();
        } else
        {
            THROW_HTTP
            (
                GpHttpResponseCode::NOT_FOUND_404,
                fmt::format
                (
                    "Handler not found for host '{}'",
                    routePathHostPort
                )
            );
        }
    }

    // Try to find path
    const auto& pathMap     = hostIter->second;
    const auto  pathIter    = pathMap.find(urlPath);

    if (pathIter == std::end(pathMap)) [[unlikely]]
    {
        if (iDefaultHandlerFactory.IsNotNULL()) [[likely]]
        {
            return iDefaultHandlerFactory.Vn().NewInstance();
        } else
        {
            THROW_HTTP
            (
                GpHttpResponseCode::NOT_FOUND_404,
                fmt::format
                (
                    "Handler not found for URL '{}'",
                    aHttpRqNoBody.url.ToString({GpUrlPartType::SCHEME, GpUrlPartType::AUTHORITY_HOST_AND_PORT, GpUrlPartType::PATH_QUERY_FRAGMENT})
                )
            );
        }
    }

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

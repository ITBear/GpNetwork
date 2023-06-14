#include "GpHttpRequestRouteTable.hpp"
#include "../Exceptions/GpHttpException.hpp"

namespace GPlatform {

GpHttpRequestRouteTable::GpHttpRequestRouteTable (void) noexcept
{
}

GpHttpRequestRouteTable::~GpHttpRequestRouteTable (void) noexcept
{
}

void    GpHttpRequestRouteTable::RegisterPath
(
    std::u8string_view              aPath,
    GpHttpRequestHandlerFactory::SP aHandlerFactory
)
{
    iHandlersCatalog.Set(aPath, std::move(aHandlerFactory));
}

GpHttpRequestHandler::SP    GpHttpRequestRouteTable::Handler (std::u8string_view aPath) const
{
    auto res = iHandlersCatalog.GetOpt(aPath);

    THROW_COND_HTTP
    (
        res.has_value(),
        GpHttpResponseCode::NOT_FOUND_404,
        [&](){return u8"Handler not found for path '"_sv + aPath + u8"'"_sv;}
    );

    return res.value().get().V().NewInstance();
}

}//namespace GPlatform

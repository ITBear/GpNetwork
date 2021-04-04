#include "GpHttpRequestRouteTable.hpp"
#include "GpHttpException.hpp"

namespace GPlatform {

GpHttpRequestRouteTable::GpHttpRequestRouteTable (void) noexcept
{
}

GpHttpRequestRouteTable::~GpHttpRequestRouteTable (void) noexcept
{
}

void    GpHttpRequestRouteTable::RegisterPath
(
    std::string_view                aPath,
    GpHttpRequestHandlerFactory::SP aHandlerFactory
)
{
    iHandlersCatalog.Register(aPath, std::move(aHandlerFactory));
}

GpHttpRequestHandler::SP    GpHttpRequestRouteTable::Handler (std::string_view aPath) const
{
    auto res = iHandlersCatalog.TryFind(aPath);

    THROW_HTTP_COND
    (
        res.has_value(),
        GpHttpResponseCode::NOT_FOUND_404,
        [&](){return "Handler not found by path '"_sv + aPath + "'"_sv;}
    );

    return res.value().get().VC().NewInstance();
}

}//namespace GPlatform

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
    std::string_view                aPath,
    GpHttpRequestHandlerFactory::SP aHandlerFactory
)
{
    iHandlersCatalog.Register(aPath, std::move(aHandlerFactory));
}

GpHttpRequestHandler::SP    GpHttpRequestRouteTable::Handler (std::string_view aPath) const
{
    auto res = iHandlersCatalog.FindOpt(aPath);

    THROW_COND_HTTP
    (
        res.has_value(),
        GpHttpResponseCode::NOT_FOUND_404,
        [&](){return "Handler not found for path '"_sv + aPath + "'"_sv;}
    );

    return res.value().get().V().NewInstance();
}

}//namespace GPlatform

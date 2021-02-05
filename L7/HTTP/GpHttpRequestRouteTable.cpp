#include "GpHttpRequestRouteTable.hpp"
#include "GpHttpException.hpp"

namespace GPlatform {

GpHttpRequestRouteTable::GpHttpRequestRouteTable (void) noexcept
{
}

GpHttpRequestRouteTable::~GpHttpRequestRouteTable (void) noexcept
{
}

void    GpHttpRequestRouteTable::RegisterPath (std::string_view                 aPath,
                                               GpHttpRequestHandlerFactory::SP  aHandlerFactory)
{
    iHandlersCatalog.Register(std::string(aPath), std::move(aHandlerFactory));
}

GpHttpRequestHandler::SP    GpHttpRequestRouteTable::Handler (std::string_view aPath) const
{
    auto res = iHandlersCatalog.Find(aPath);

    if (!res.has_value())
    {
        THROW_HTTP(GpHttpResponseCode::NOT_FOUND_404,
                   "Handler not found by path '"_sv + aPath + "'"_sv);
    }

    return res.value().get().VC().NewInstance();
}

}//namespace GPlatform

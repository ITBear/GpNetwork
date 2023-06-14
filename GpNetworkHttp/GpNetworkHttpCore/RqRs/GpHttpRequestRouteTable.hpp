#pragma once

#include "GpHttpRequestHandler.hpp"
#include "GpHttpRequestHandlerFactory.hpp"
#include "../../../../GpCore2/GpUtils/Types/Containers/GpDictionary.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestRouteTable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestRouteTable)
    CLASS_DD(GpHttpRequestRouteTable)

    using HandlersCatalogT = GpDictionary<std::u8string, GpHttpRequestHandlerFactory::SP>;

public:
                                GpHttpRequestRouteTable     (void) noexcept;
                                ~GpHttpRequestRouteTable    (void) noexcept;

    void                        RegisterPath                (std::u8string_view                 aPath,
                                                             GpHttpRequestHandlerFactory::SP    aHandlerFactory);
    GpHttpRequestHandler::SP    Handler                     (std::u8string_view aPath) const;

private:
    HandlersCatalogT            iHandlersCatalog;
};

}//namespace GPlatform

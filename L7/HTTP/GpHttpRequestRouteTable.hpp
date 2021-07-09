#pragma once

#include "GpHttpRequestHandler.hpp"
#include "GpHttpRequestHandlerFactory.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequestRouteTable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestRouteTable)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestRouteTable)

    using HandlersCatalogT = GpElementsCatalog<std::string, GpHttpRequestHandlerFactory::SP>;

public:
                                GpHttpRequestRouteTable     (void) noexcept;
                                ~GpHttpRequestRouteTable    (void) noexcept;

    void                        RegisterPath                (std::string_view                   aPath,
                                                             GpHttpRequestHandlerFactory::SP    aHandlerFactory);
    GpHttpRequestHandler::SP    Handler                     (std::string_view aPath) const;

private:
    HandlersCatalogT            iHandlersCatalog;
};

}//namespace GPlatform

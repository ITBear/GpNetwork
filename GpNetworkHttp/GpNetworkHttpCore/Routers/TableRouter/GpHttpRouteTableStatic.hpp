#pragma once

#include "GpHttpRouteTable.hpp"
#include "../../RequestHandlers/GpHttpRequestHandlerFactory.hpp"

#include <GpCore2/GpUtils/SyncPrimitives/GpRWSpinLock.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRouteTableStatic final: public GpHttpRouteTable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRouteTableStatic)
    CLASS_DD(GpHttpRouteTableStatic)
    TAG_SET(THREAD_SAFE)

    using HandlerFactoryCatalogT = std::map<std::u8string/*host*/, std::map<std::u8string/*path*/, GpHttpRequestHandlerFactory::SP, std::less<>>, std::less<>>;

public:
                                        GpHttpRouteTableStatic  (void) noexcept;
    virtual                             ~GpHttpRouteTableStatic (void) noexcept override final;

    virtual GpHttpRequestHandler::SP    FindHandler             (const GpHttpRequestNoBodyDesc& aHttpRqNoBody) const override final;

    void                                RegisterDefaultHandler  (GpHttpRequestHandlerFactory::SP    aHandlerFactory);

    void                                RegisterPathHandler     (std::u8string_view                 aHost,
                                                                 std::u8string_view                 aPath,
                                                                 GpHttpRequestHandlerFactory::SP    aHandlerFactory);

private:
    mutable GpRWSpinLock                iLock;
    HandlerFactoryCatalogT              iRouteRable;
    GpHttpRequestHandlerFactory::SP     iDefaultHandlerFactory;
};

}//namespace GPlatform

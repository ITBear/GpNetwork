#pragma once

#include "GpHttpRouteTable.hpp"
#include "../../RequestHandlers/GpHttpRequestHandlerFactory.hpp"

#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRouteTableStatic final: public GpHttpRouteTable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRouteTableStatic)
    CLASS_DD(GpHttpRouteTableStatic)
    TAG_SET(THREAD_SAFE)

    using HandlerFactoryCatalogT = boost::container::flat_map
    <
        std::string/*host*/,
        boost::container::flat_map
        <
            std::string/*path*/,
            GpHttpRequestHandlerFactory::SP,
            std::less<>
        >,
        std::less<>
    >;

public:
                                        GpHttpRouteTableStatic  (void) noexcept;
    virtual                             ~GpHttpRouteTableStatic (void) noexcept override final;

    virtual GpHttpRequestHandler::SP    FindHandler             (const GpHttpRequestNoBodyDesc& aHttpRqNoBody) const override final;

    void                                RegisterDefaultHandler  (GpHttpRequestHandlerFactory::SP    aHandlerFactory);
    void                                RegisterPathHandler     (std::string                        aHost,
                                                                 std::string                        aPath,
                                                                 GpHttpRequestHandlerFactory::SP    aHandlerFactory);

private:
    mutable GpSpinLockRW                iSpinLockRW;
    HandlerFactoryCatalogT              iRouteRable             GUARDED_BY(iSpinLockRW);
    GpHttpRequestHandlerFactory::SP     iDefaultHandlerFactory  GUARDED_BY(iSpinLockRW);
};

}// namespace GPlatform

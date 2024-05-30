#pragma once

#include "../GpHttpRouter.hpp"
#include "GpHttpRouteTable.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpTableRouter final: public GpHttpRouter
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpTableRouter)
    CLASS_DD(GpHttpTableRouter)
    TAG_SET(THREAD_SAFE)

public:
    inline                              GpHttpTableRouter   (GpHttpRouteTable::SP aRouteTable) noexcept;
    virtual                             ~GpHttpTableRouter  (void) noexcept override final;

    virtual GpHttpRequestHandler::SP    FindHandler         (const GpHttpRequestNoBodyDesc& aHttpRqNoBody) override final;

private:
    GpHttpRouteTable::SP                iRouteTable;
};

GpHttpTableRouter::GpHttpTableRouter (GpHttpRouteTable::SP aRouteTable) noexcept:
iRouteTable(std::move(aRouteTable))
{
}

}// namespace GPlatform

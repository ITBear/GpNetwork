#pragma once

#include "../../RequestHandlers/GpHttpRequestHandler.hpp"

namespace GPlatform {

class GpHttpRouteTable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRouteTable)
    CLASS_DD(GpHttpRouteTable)
    TAG_SET(THREAD_SAFE)

protected:
                                        GpHttpRouteTable    (void) noexcept = default;

public:
    virtual                             ~GpHttpRouteTable   (void) noexcept = default;

    virtual GpHttpRequestHandler::SP    FindHandler         (const GpHttpRequestNoBodyDesc& aHttpRqNoBody) const = 0;
};

}// namespace GPlatform

#pragma once

#include "../../RequestHandlers/GpHttpRequestHandler.hpp"

namespace GPlatform {

class GpHttpRouteTable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRouteTable)
    CLASS_DD(GpHttpRouteTable)
    TAG_SET(THREAD_SAFE)

public:
                                        GpHttpRouteTable    (void) noexcept = default;
    virtual                             ~GpHttpRouteTable   (void) noexcept = default;

    virtual GpHttpRequestHandler::SP    FindHandler         (const GpHttpRequestNoBodyDesc& aHttpRqNoBody) const = 0;
};

}//namespace GPlatform

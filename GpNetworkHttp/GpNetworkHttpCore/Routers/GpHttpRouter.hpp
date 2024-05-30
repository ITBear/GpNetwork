#pragma once

#include "../RequestHandlers/GpHttpRequestHandler.hpp"

namespace GPlatform {

class GpHttpRouter
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRouter)
    CLASS_DD(GpHttpRouter)
    TAG_SET(THREAD_SAFE)

protected:
                                        GpHttpRouter    (void) noexcept = default;

public:
    virtual                             ~GpHttpRouter   (void) noexcept = default;

    virtual GpHttpRequestHandler::SP    FindHandler     (const GpHttpRequestNoBodyDesc& aHttpRqNoBody) = 0;
};

}// namespace GPlatform

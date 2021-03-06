#pragma once

#include "GpHttpClientPool.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpClientGuard
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClientGuard)
    CLASS_DECLARE_DEFAULTS(GpHttpClientGuard)

public:
                            GpHttpClientGuard   (GpHttpClientPool& aPool) noexcept;
                            ~GpHttpClientGuard  (void) noexcept;

    GpHttpClient&           Client              (void);

private:
    GpHttpClientPool&       iPool;
    GpHttpClient::SP        iHttpClient;
};

}//GPlatform

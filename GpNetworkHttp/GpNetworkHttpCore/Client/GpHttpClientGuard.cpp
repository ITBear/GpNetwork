#include "GpHttpClientGuard.hpp"

namespace GPlatform {

GpHttpClientGuard::GpHttpClientGuard (GpHttpClientPool& aPool) noexcept:
iPool(aPool)
{
}

GpHttpClientGuard::~GpHttpClientGuard (void) noexcept
{
    if (iHttpClient.IsNULL())
    {
        return;
    }

    iPool.Release(iHttpClient);
}

GpHttpClient&   GpHttpClientGuard::Client (void)
{
    if (iHttpClient.IsNULL())
    {
        auto res = iPool.Acquire();
        THROW_COND_GP(res.has_value(), "HTTP clients limit exceeded"_sv);
        iHttpClient = res.value();
    }

    return iHttpClient.Vn();
}

}//namespace GPlatform

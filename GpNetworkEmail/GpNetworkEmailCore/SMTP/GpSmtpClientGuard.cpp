#include "GpSmtpClientGuard.hpp"

namespace GPlatform {

GpSmtpClientGuard::GpSmtpClientGuard (GpSmtpClientPool& aPool) noexcept:
iPool(aPool)
{
}

GpSmtpClientGuard::~GpSmtpClientGuard (void) noexcept
{
    if (iSmtpClient.IsNULL())
    {
        return;
    }

    iPool.Release(iSmtpClient);
}

GpSmtpClient&   GpSmtpClientGuard::Client (void)
{
    if (iSmtpClient.IsNULL())
    {
        auto res = iPool.Acquire();
        THROW_COND_GP(res.has_value(), "Smtp clients limit exceeded"_sv);
        iSmtpClient = res.value();
    }

    return iSmtpClient.Vn();
}

}//namespace GPlatform

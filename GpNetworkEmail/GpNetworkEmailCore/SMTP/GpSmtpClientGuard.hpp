#pragma once

#include "GpSmtpClient.hpp"
#include "GpSmtpClientPool.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpSmtpClientGuard
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSmtpClientGuard)
    CLASS_DD(GpSmtpClientGuard)

public:
                            GpSmtpClientGuard   (GpSmtpClientPool& aPool) noexcept;
                            ~GpSmtpClientGuard  (void) noexcept;

    GpSmtpClient&           Client              (void);

private:
    GpSmtpClientPool&       iPool;
    GpSmtpClient::SP        iSmtpClient;
};

}//GPlatform

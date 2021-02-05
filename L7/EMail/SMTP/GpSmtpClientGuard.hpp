#pragma once

#include "GpSmtpClient.hpp"
#include "GpSmtpClientPool.hpp"

namespace GPlatform {

class GPNETWORK_API GpSmtpClientGuard
{
public:
    CLASS_REMOVE_CTRS(GpSmtpClientGuard)
    CLASS_DECLARE_DEFAULTS(GpSmtpClientGuard)

public:
                            GpSmtpClientGuard   (GpSmtpClientPool& aPool) noexcept;
                            ~GpSmtpClientGuard  (void) noexcept;

    GpSmtpClient&           Client              (void);

private:
    GpSmtpClientPool&       iPool;
    GpSmtpClient::SP        iSmtpClient;
};

}//GPlatform

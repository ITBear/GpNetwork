#pragma once

#include "GpSmtpClient.hpp"

namespace GPlatform {

class GPNETWORK_API GpSmtpClientFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSmtpClientFactory)
    CLASS_DECLARE_DEFAULTS(GpSmtpClientFactory)

protected:
                                GpSmtpClientFactory     (void) noexcept {}

public:
    virtual                     ~GpSmtpClientFactory    (void) noexcept {}

    virtual GpSmtpClient::SP    NewInstance             (void) const = 0;
};

}//namespace GPlatform

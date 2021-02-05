#pragma once

#include "GpSmtpClientFactory.hpp"

namespace GPlatform {

class GPNETWORK_API GpSmtpClientCurlFactory final: public GpSmtpClientFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpSmtpClientCurlFactory)
    CLASS_DECLARE_DEFAULTS(GpSmtpClientCurlFactory)

public:
                                GpSmtpClientCurlFactory     (void) noexcept {}
    virtual                     ~GpSmtpClientCurlFactory    (void) noexcept override final {}

    virtual GpSmtpClient::SP    NewInstance                 (void) const override final;
};

}//namespace GPlatform

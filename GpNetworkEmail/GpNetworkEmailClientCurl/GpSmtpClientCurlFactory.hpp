#pragma once

#include "GpNetworkEmailClientCurl_global.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CLIENT_CURL_API GpSmtpClientCurlFactory final: public GpSmtpClientFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSmtpClientCurlFactory)
    CLASS_DD(GpSmtpClientCurlFactory)

public:
                                GpSmtpClientCurlFactory     (void) noexcept {}
    virtual                     ~GpSmtpClientCurlFactory    (void) noexcept override final {}

    virtual GpSmtpClient::SP    NewInstance                 (void) const override final;
};

}//namespace GPlatform

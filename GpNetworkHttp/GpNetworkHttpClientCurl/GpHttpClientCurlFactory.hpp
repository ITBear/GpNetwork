#pragma once

#include "GpNetworkHttpClientCurl_global.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CLIENT_CURL_API GpHttpClientCurlFactory final: public GpHttpClientFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpClientCurlFactory)
    CLASS_DD(GpHttpClientCurlFactory)

public:
                                GpHttpClientCurlFactory     (void) noexcept = default;
    virtual                     ~GpHttpClientCurlFactory    (void) noexcept override final = default;

    virtual GpHttpClient::SP    NewInstance                 (void) const override final;
};

}//namespace GPlatform

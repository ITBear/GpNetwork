#pragma once

#include "GpNetworkHttpClientCurl_global.hpp"
#include "../GpNetworkHttpCore/Client/GpHttpClientFactory.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CLIENT_CURL_API GpHttpClientCurlAsyncFactory final: public GpHttpClientFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpClientCurlAsyncFactory)
    CLASS_DD(GpHttpClientCurlAsyncFactory)

public:
                                GpHttpClientCurlAsyncFactory    (void) noexcept;
    virtual                     ~GpHttpClientCurlAsyncFactory   (void) noexcept override final;

    virtual GpHttpClient::SP    NewInstance                     (GpSocketFlags      aSocketFlags,
                                                                 GpIOEventPollerIdx aIOEventPollerIdx,
                                                                 milliseconds_t     aConnectTimeout) const override final;
    virtual GpHttpClient::SP    NewInstance                     (GpSocketFlags      aSocketFlags,
                                                                 GpIOEventPollerIdx aIOEventPollerIdx,
                                                                 milliseconds_t     aConnectTimeout,
                                                                 std::string        aTaskName) const override final;
private:
    const std::string           iIoEventPollerName;
};

}// namespace GPlatform

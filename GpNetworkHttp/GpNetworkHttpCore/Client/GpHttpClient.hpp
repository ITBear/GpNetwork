#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpTcpAcceptServerTask.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Client/GpHttpClientRequestTask.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpClient
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClient)
    CLASS_DD(GpHttpClient)
    TAG_SET(THREAD_SAFE)

public:
                                GpHttpClient    (GpSocketFlags      aSocketFlags,
                                                 GpIOEventPollerIdx aIOEventPollerIdx) noexcept;
                                ~GpHttpClient   (void) noexcept;

    GpHttpResponse::SP          DoAndWait       (GpHttpRequest::SP  aRequestSP,
                                                 milliseconds_t     aConnectTimeout,
                                                 milliseconds_t     aRequestTimeout);

private:
    const GpSocketFlags         iSocketFlags;
    const GpIOEventPollerIdx    iIOEventPollerIdx;
};

}// namespace GPlatform

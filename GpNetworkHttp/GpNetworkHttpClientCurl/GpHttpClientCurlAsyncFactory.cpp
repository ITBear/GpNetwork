#include "GpHttpClientCurlAsyncFactory.hpp"
#include "GpHttpClientCurlAsync.hpp"

namespace GPlatform {

GpHttpClientCurlAsyncFactory::GpHttpClientCurlAsyncFactory (void) noexcept
{
}

GpHttpClientCurlAsyncFactory::~GpHttpClientCurlAsyncFactory (void) noexcept
{
}

GpHttpClient::SP    GpHttpClientCurlAsyncFactory::NewInstance
(
    GpSocketFlags       aSocketFlags,
    GpIOEventPollerIdx  aIOEventPollerIdx,
    milliseconds_t      aConnectTimeout
) const
{
    return MakeSP<GpHttpClientCurlAsync>
    (
        aSocketFlags,
        aIOEventPollerIdx,
        aConnectTimeout
    );
}

GpHttpClient::SP    GpHttpClientCurlAsyncFactory::NewInstance
(
    GpSocketFlags       aSocketFlags,
    GpIOEventPollerIdx  aIOEventPollerIdx,
    milliseconds_t      aConnectTimeout,
    std::string         aTaskName
) const
{
    return MakeSP<GpHttpClientCurlAsync>
    (
        aSocketFlags,
        aIOEventPollerIdx,
        aConnectTimeout,
        std::move(aTaskName)
    );
}

}// namespace GPlatform

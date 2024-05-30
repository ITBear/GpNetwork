#include "GpHttpClient.hpp"

namespace GPlatform {

GpHttpClient::GpHttpClient
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout
):
GpTcpClientTask
{
    aSocketFlags,
    aIOEventPollerIdx,
    aConnectTimeout
}
{
}

GpHttpClient::GpHttpClient
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout,
    std::string                 aTaskName
):
GpTcpClientTask
{
    aSocketFlags,
    aIOEventPollerIdx,
    aConnectTimeout,
    std::move(aTaskName)
}
{
}

GpHttpClient::~GpHttpClient (void) noexcept
{
}

}// namespace GPlatform

#pragma once

#include "GpHttpClient.hpp"

namespace GPlatform {

class GpHttpClientFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpClientFactory)
    CLASS_DD(GpHttpClientFactory)

protected:
                                GpHttpClientFactory     (void) noexcept = default;

public:
    virtual                     ~GpHttpClientFactory    (void) noexcept = default;

    virtual GpHttpClient::SP    NewInstance             (GpSocketFlags      aSocketFlags,
                                                         GpIOEventPollerIdx aIOEventPollerIdx,
                                                         milliseconds_t     aConnectTimeout) const = 0;

    virtual GpHttpClient::SP    NewInstance             (GpSocketFlags      aSocketFlags,
                                                         GpIOEventPollerIdx aIOEventPollerIdx,
                                                         milliseconds_t     aConnectTimeout,
                                                         std::string        aTaskName) const = 0;
};

}// namespace GPlatform

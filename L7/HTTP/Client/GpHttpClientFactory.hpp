#pragma once

#include "GpHttpClient.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpClientFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpClientFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpClientFactory)

protected:
                                GpHttpClientFactory     (void) noexcept {}

public:
    virtual                     ~GpHttpClientFactory    (void) noexcept {}

    virtual GpHttpClient::SP    NewInstance             (void) const = 0;
};

}//namespace GPlatform

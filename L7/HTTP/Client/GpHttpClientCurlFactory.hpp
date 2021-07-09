#pragma once

#include "GpHttpClientFactory.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpClientCurlFactory final: public GpHttpClientFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpClientCurlFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpClientCurlFactory)

public:
                                GpHttpClientCurlFactory     (void) noexcept {}
    virtual                     ~GpHttpClientCurlFactory    (void) noexcept override final {}

    virtual GpHttpClient::SP    NewInstance                 (void) const override final;
};

}//namespace GPlatform

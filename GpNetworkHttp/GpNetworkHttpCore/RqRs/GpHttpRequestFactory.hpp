#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpRequest.hpp>

namespace GPlatform {

class GpHttpRequestFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestFactory)
    CLASS_DD(GpHttpRequestFactory)

protected:
                                GpHttpRequestFactory    (void) noexcept = default;
public:
    virtual                     ~GpHttpRequestFactory   (void) noexcept = default;

    virtual GpHttpRequest::SP   NewInstance             (void) const  = 0;
};

}// namespace GPlatform

#pragma once

#include "GpHttpRequest.hpp"

namespace GPlatform {

class GpHttpRequestSerializer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpRequestSerializer)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestSerializer)

public:
                                GpHttpRequestSerializer     (GpHttpRequest::SP aRequest) noexcept;
                                ~GpHttpRequestSerializer    (void) noexcept;

private:
    GpHttpRequest::SP           iRequest;
};

}//namespace GPlatform

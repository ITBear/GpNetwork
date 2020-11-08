#pragma once

#include "GpHttpRequest.hpp"

namespace GPlatform {

class GpHttpRequestSerializer
{
public:
    CLASS_REMOVE_CTRS(GpHttpRequestSerializer)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestSerializer)

public:
                                GpHttpRequestSerializer     (GpHttpRequest::SP aRequest) noexcept;
                                ~GpHttpRequestSerializer    (void) noexcept;

private:
    GpHttpRequest::SP           iRequest;
};

}//namespace GPlatform

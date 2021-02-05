#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpClient
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpClient)
    CLASS_DECLARE_DEFAULTS(GpHttpClient)

    enum class ErorrMode
    {
        THROW_ON_NOT_200,
        RETURN_RS
    };

public:
                                    GpHttpClient    (void) noexcept {}
    virtual                         ~GpHttpClient   (void) noexcept {}

    virtual GpHttpResponse::SP      Do              (GpHttpRequest::SP  aRequest,
                                                     const ErorrMode    aErorrMode) = 0;

    virtual bool                    IsValid         (void) const noexcept = 0;
};

}//namespace GPlatform

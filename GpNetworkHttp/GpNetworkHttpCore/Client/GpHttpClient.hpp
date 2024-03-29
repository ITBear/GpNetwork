#pragma once

#include "../RqRs/GpHttpRqRs.hpp"

namespace GPlatform {

class GpHttpClient
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpClient)
    CLASS_DD(GpHttpClient)

    enum class ErorrMode
    {
        THROW_ON_NOT_200,
        RETURN_RS
    };

public:
                                    GpHttpClient    (void) noexcept = default;
    virtual                         ~GpHttpClient   (void) noexcept = default;

    virtual GpHttpResponse::SP      Do              (GpHttpRequest::SP  aRequest,
                                                     const ErorrMode    aErorrMode) = 0;

    virtual bool                    IsValid         (void) const noexcept = 0;
};

}//namespace GPlatform

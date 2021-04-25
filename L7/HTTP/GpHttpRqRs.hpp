#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRqRs
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT_MOVE(GpHttpRqRs)
    CLASS_DECLARE_DEFAULTS(GpHttpRqRs)

public:
                            GpHttpRqRs      (void) noexcept;
                            GpHttpRqRs      (GpHttpRqRs&& aRqRs) noexcept;
                            GpHttpRqRs      (GpHttpRequest::SP  aRq,
                                             GpHttpResponse::SP aRs) noexcept;
                            ~GpHttpRqRs     (void) noexcept;

    GpHttpRequest::SP       Rq              (void) noexcept {return iRq;}
    GpHttpResponse::SP      Rs              (void) noexcept {return iRs;}

    inline void             SetRqRs         (GpHttpRequest::SP  aRq,
                                             GpHttpResponse::SP aRs) noexcept;

private:
    GpHttpRequest::SP       iRq;
    GpHttpResponse::SP      iRs;
};

void    GpHttpRqRs::SetRqRs
(
    GpHttpRequest::SP   aRq,
    GpHttpResponse::SP  aRs
) noexcept
{
    iRq = std::move(aRq);
    iRs = std::move(aRs);
}

}//namespace GPlatform

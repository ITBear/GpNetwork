#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"
#include "GpHttpRequestHandlerFactory.hpp"
#include "GpHttpRequestDoneEvent.hpp"
#include "GpHttpRequestTask.hpp"
#include "GpHttpResponseSerializer.hpp"
#include "GpUrlReflection.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRqRs
{
public:
    CLASS_REMOVE_CTRS_COPY(GpHttpRqRs)
    CLASS_DD(GpHttpRqRs)

public:
                            GpHttpRqRs      (void) noexcept;
                            GpHttpRqRs      (GpHttpRqRs&& aRqRs) noexcept;
                            GpHttpRqRs      (GpHttpRequest::SP  aRq,
                                             GpHttpResponse::SP aRs) noexcept;
                            ~GpHttpRqRs     (void) noexcept;

    GpHttpRequest::SP       Rq              (void) noexcept {return iRq;}
    GpHttpResponse::SP      Rs              (void) noexcept {return iRs;}

    inline void             SetRq           (GpHttpRequest::SP  aRq) noexcept;
    inline void             SetRs           (GpHttpResponse::SP aRs) noexcept;

    inline void             SetRqRs         (GpHttpRequest::SP  aRq,
                                             GpHttpResponse::SP aRs) noexcept;

private:
    GpHttpRequest::SP       iRq;
    GpHttpResponse::SP      iRs;
};

void    GpHttpRqRs::SetRq (GpHttpRequest::SP aRq) noexcept
{
    iRq = std::move(aRq);
}

void    GpHttpRqRs::SetRs (GpHttpResponse::SP aRs) noexcept
{
    iRs = std::move(aRs);
}

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

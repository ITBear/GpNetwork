#include "GpHttpRqRs.hpp"

namespace GPlatform {

GpHttpRqRs::GpHttpRqRs (void) noexcept
{
}

GpHttpRqRs::GpHttpRqRs (GpHttpRqRs&& aRqRs) noexcept:
iRq(std::move(aRqRs.iRq)),
iRs(std::move(aRqRs.iRs))
{
}

GpHttpRqRs::GpHttpRqRs
(
    GpHttpRequest::SP   aRq,
    GpHttpResponse::SP  aRs
) noexcept:
iRq(std::move(aRq)),
iRs(std::move(aRs))
{
}

GpHttpRqRs::~GpHttpRqRs (void) noexcept
{
}

}//namespace GPlatform

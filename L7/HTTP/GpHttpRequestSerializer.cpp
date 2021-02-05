#include "GpHttpRequestSerializer.hpp"

namespace GPlatform {

GpHttpRequestSerializer::GpHttpRequestSerializer (GpHttpRequest::SP aRequest) noexcept:
iRequest(std::move(aRequest))
{
}

GpHttpRequestSerializer::~GpHttpRequestSerializer (void) noexcept
{
}

}//namespace GPlatform

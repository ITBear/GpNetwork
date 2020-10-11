#include "GpHttpRequest.hpp"

namespace GPlatform {

GpHttpRequest::GpHttpRequest (void) noexcept
{
}

GpHttpRequest::GpHttpRequest (std::string_view  aUrl,
                              std::string_view  aContentType,
                              GpBytesArray&&    aBody):
iUrl(aUrl),
iContentType(aContentType),
iBody(std::move(aBody))
{
}

GpHttpRequest::~GpHttpRequest (void) noexcept
{
}

}//namespace GPlatform

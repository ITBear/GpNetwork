#include "GpHttpResponse.hpp"

namespace GPlatform {

GpHttpResponse::GpHttpResponse (void) noexcept
{
}

GpHttpResponse::GpHttpResponse (CodeTE              aCode,
                                std::string_view    aContentType,
                                GpBytesArray&&      aBody):
iCode(aCode),
iContentType(aContentType),
iBody(std::move(aBody))
{
}

GpHttpResponse::~GpHttpResponse (void) noexcept
{
}

}//namespace GPlatform

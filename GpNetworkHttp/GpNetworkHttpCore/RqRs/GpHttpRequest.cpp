#include "GpHttpRequest.hpp"
#include "../Exceptions/GpHttpException.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpHttpRequest, GP_MODULE_UUID)

GpHttpRequest::~GpHttpRequest (void) noexcept
{
}

void    GpHttpRequest::SetHttpVersion (const size_t aMajor, const size_t aMinor)
{
    if ((aMajor == 1) && (aMinor == 0))
    {
        http_version = HttpVersionTE::HTTP_1_0;
    } else if ((aMajor == 1) && (aMinor == 1))
    {
        http_version = HttpVersionTE::HTTP_1_1;
    } else
    {
        THROW_HTTP(GpHttpResponseCode::BAD_REQUEST_400, "Unsupported HTTP version"_sv);
    }
}

void    GpHttpRequest::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(http_version);
    PROP(request_type);
    PROP(url);
    PROP(headers);
    PROP(body);
}

}//namespace GPlatform

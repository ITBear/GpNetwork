#include "GpHttpRequestNoBodyDesc.hpp"
#include "../Exceptions/GpHttpException.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpHttpRequestNoBodyDesc, GP_MODULE_UUID)

GpHttpRequestNoBodyDesc::~GpHttpRequestNoBodyDesc (void) noexcept
{
}

void    GpHttpRequestNoBodyDesc::SetHttpVersion
(
    const size_t aMajor,
    const size_t aMinor
)
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

void    GpHttpRequestNoBodyDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(http_version);
    PROP(request_type);
    PROP(url);
    PROP(headers);
}

}//namespace GPlatform

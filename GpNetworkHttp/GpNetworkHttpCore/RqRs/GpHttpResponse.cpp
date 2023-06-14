#include "GpHttpResponse.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectManager.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpHttpResponse, GP_MODULE_UUID)

GpHttpResponse::~GpHttpResponse (void) noexcept
{
}

void    GpHttpResponse::SetFromException (const GpHttpException& aHttpEx)
{
    code    = aHttpEx.Code();
    body    = GpBytesArrayUtils::SMake(GpUTF::S_STR_To_UTF8(aHttpEx.what()));

    headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::SetFromException (const GpException& aEx)
{
    code    = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    body    = GpBytesArrayUtils::SMake(GpUTF::S_STR_To_UTF8(aEx.what()));

    headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::SetFromException (const std::exception& aEx)
{
    code    = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    body    = GpBytesArrayUtils::SMake(GpUTF::S_STR_To_UTF8(aEx.what()));

    headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(http_version);
    PROP(code);
    PROP(headers);
    PROP(body);
}

}//namespace GPlatform

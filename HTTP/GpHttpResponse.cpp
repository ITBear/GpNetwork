#include "GpHttpResponse.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpHttpResponse, GP_MODULE_UUID)

GpHttpResponse::GpHttpResponse (void) noexcept
{
}

GpHttpResponse::GpHttpResponse (const CodeTE            aCode,
                                const GpHttpHeaders&    aHeaders):
code(aCode),
headers(aHeaders)
{
}

GpHttpResponse::GpHttpResponse (const CodeTE        aCode,
                                GpHttpHeaders&&     aHeaders) noexcept:
code(aCode),
headers(std::move(aHeaders))
{
}

GpHttpResponse::GpHttpResponse (const CodeTE            aCode,
                                const GpHttpHeaders&    aHeaders,
                                GpBytesArray&&          aBody):
code(aCode),
headers(aHeaders),
body(std::move(aBody))
{
}

GpHttpResponse::GpHttpResponse (const CodeTE    aCode,
                                GpHttpHeaders&& aHeaders,
                                GpBytesArray&&  aBody) noexcept:
code(aCode),
headers(std::move(aHeaders)),
body(std::move(aBody))
{
}

GpHttpResponse::~GpHttpResponse (void) noexcept
{
}

void    GpHttpResponse::SetFromException (const GpHttpException& aHttpEx)
{
    code    = aHttpEx.Code();
    body    = std::move(GpBytesArrayUtils::SMake(std::string_view(aHttpEx.what())));

    headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .CacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::SetFromException (const GpException& aEx)
{
    code    = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    body    = std::move(GpBytesArrayUtils::SMake(std::string_view(aEx.what())));

    headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .CacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::SetFromException (const std::exception& aEx)
{
    code            = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    body            = std::move(GpBytesArrayUtils::SMake(std::string_view(aEx.what())));

    headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .CacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(http_version);
    PROP(code);
    PROP(headers);
    PROP(body);
}

}//namespace GPlatform

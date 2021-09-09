#include "GpHttpResponse.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpHttpResponse, GP_MODULE_UUID)

GpHttpResponse::GpHttpResponse (void) noexcept
{
}

GpHttpResponse::GpHttpResponse (const GpHttpResponse& aResponse):
http_version(aResponse.http_version),
code(aResponse.code),
headers(aResponse.headers),
body(aResponse.body)
{
}

GpHttpResponse::GpHttpResponse (GpHttpResponse&& aResponse) noexcept:
http_version(std::move(aResponse.http_version)),
code(std::move(aResponse.code)),
headers(std::move(aResponse.headers)),
body(std::move(aResponse.body))
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE            aCode,
    const GpHttpHeaders&    aHeaders
):
code(aCode),
headers(aHeaders)
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE        aCode,
    GpHttpHeaders&&     aHeaders
) noexcept:
code(aCode),
headers(std::move(aHeaders))
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE            aCode,
    const GpHttpHeaders&    aHeaders,
    GpBytesArray&&          aBody
):
code(aCode),
headers(aHeaders),
body(std::move(aBody))
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE    aCode,
    GpHttpHeaders&& aHeaders,
    GpBytesArray&&  aBody
) noexcept:
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
    body    = GpBytesArrayUtils::SMake(std::string_view(aHttpEx.what()));

    headers
        .SetContentType(GpContentType::TEXT_PLAIN, GpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::SetFromException (const GpException& aEx)
{
    code    = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    body    = GpBytesArrayUtils::SMake(std::string_view(aEx.what()));

    headers
        .SetContentType(GpContentType::TEXT_PLAIN, GpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::SetFromException (const std::exception& aEx)
{
    code    = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    body    = GpBytesArrayUtils::SMake(std::string_view(aEx.what()));

    headers
        .SetContentType(GpContentType::TEXT_PLAIN, GpCharset::UTF_8)
        .SetConnection(GpHttpConnectionFlag::CLOSE)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);
}

void    GpHttpResponse::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(http_version);
    PROP(code);
    PROP(headers);
    PROP(body);
}

}//namespace GPlatform

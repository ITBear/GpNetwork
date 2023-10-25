#include "GpHttpResponse.hpp"
#include "../Body/GpHttpBodyPayloadFixed.hpp"

namespace GPlatform {

GpHttpResponse::~GpHttpResponse (void) noexcept
{
}

GpHttpResponse::SP  GpHttpResponse::SFromException
(
    const GpHttpException&          aHttpEx,
    const GpHttpRequestNoBodyDesc&  aRqDesc
)
{
    GpHttpResponseNoBodyDesc rsDesc;

    rsDesc.code         = aHttpEx.Code();
    rsDesc.http_version = aRqDesc.http_version;
    rsDesc.headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);

    return MakeSP<GpHttpResponse>
    (
        std::move(rsDesc),
        MakeSP<GpHttpBodyPayloadFixed>(GpBytesArrayUtils::SConvert(GpUTF::S_STR_To_UTF8(aHttpEx.what())))
    );
}

GpHttpResponse::SP  GpHttpResponse::SFromException
(
    const GpException&              aEx,
    const GpHttpRequestNoBodyDesc&  aRqDesc
)
{
    GpHttpResponseNoBodyDesc rsDesc;

    rsDesc.code         = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    rsDesc.http_version = aRqDesc.http_version;
    rsDesc.headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);

    return MakeSP<GpHttpResponse>
    (
        std::move(rsDesc),
        MakeSP<GpHttpBodyPayloadFixed>(GpBytesArrayUtils::SConvert(GpUTF::S_STR_To_UTF8(aEx.what())))
    );
}

GpHttpResponse::SP  GpHttpResponse::SFromException
(
    const std::exception&           aEx,
    const GpHttpRequestNoBodyDesc&  aRqDesc
)
{
    GpHttpResponseNoBodyDesc rsDesc;

    rsDesc.code         = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    rsDesc.http_version = aRqDesc.http_version;
    rsDesc.headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);

    return MakeSP<GpHttpResponse>
    (
        std::move(rsDesc),
        MakeSP<GpHttpBodyPayloadFixed>(GpBytesArrayUtils::SConvert(GpUTF::S_STR_To_UTF8(aEx.what())))
    );
}

}//namespace GPlatform

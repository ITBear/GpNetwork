#include "GpHttpResponse.hpp"
#include "../Body/GpHttpBodyPayloadFixed.hpp"

namespace GPlatform {

GpHttpResponse::~GpHttpResponse (void) noexcept
{
}

GpHttpResponse::SP  GpHttpResponse::SFromException
(
    const GpHttpException&      aHttpEx,
    const GpHttpVersion::EnumT  aHttpVersion
)
{
    GpHttpResponseNoBodyDesc rsDesc;

    rsDesc.code         = aHttpEx.Code();
    rsDesc.http_version = aHttpVersion;
    rsDesc.headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);

    return MakeSP<GpHttpResponse>
    (
        std::move(rsDesc),
        MakeSP<GpHttpBodyPayloadFixed>(GpBytesArrayUtils::SMake(std::string_view(aHttpEx.what())))
    );
}

GpHttpResponse::SP  GpHttpResponse::SFromException
(
    const GpException&          aEx,
    const GpHttpVersion::EnumT  aHttpVersion
)
{
    GpHttpResponseNoBodyDesc rsDesc;

    rsDesc.code         = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    rsDesc.http_version = aHttpVersion;
    rsDesc.headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);

    return MakeSP<GpHttpResponse>
    (
        std::move(rsDesc),
        MakeSP<GpHttpBodyPayloadFixed>(GpBytesArrayUtils::SMake(std::string_view(aEx.what())))
    );
}

GpHttpResponse::SP  GpHttpResponse::SFromException
(
    const std::exception&       aEx,
    const GpHttpVersion::EnumT  aHttpVersion
)
{
    GpHttpResponseNoBodyDesc rsDesc;

    rsDesc.code         = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    rsDesc.http_version = aHttpVersion;
    rsDesc.headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetCacheControl(GpHttpCacheControl::NO_STORE);

    return MakeSP<GpHttpResponse>
    (
        std::move(rsDesc),
        MakeSP<GpHttpBodyPayloadFixed>(GpBytesArrayUtils::SMake(std::string_view(aEx.what())))
    );
}

GpHttpResponse::SerializeRes    GpHttpResponse::SSerialize
(
    const GpHttpResponse&   aHttpResponse,
    GpByteWriter&           aWriter
)
{
    const GpHttpResponseNoBodyDesc& responseNoBody = aHttpResponse.iResponseNoBody;

    // HTTP version
    switch (responseNoBody.http_version.Value())
    {
        case GpHttpVersion::HTTP_1_0:
        {
            aWriter.Bytes("HTTP/1.0"_sv);
        } break;
        case GpHttpVersion::HTTP_1_1:
        {
            aWriter.Bytes("HTTP/1.1"_sv);
        } break;
        default:
        {
            // NOP
        }
    }

    // Responce code and message
    {
        aWriter.Bytes(" "_sv).Bytes(SGpHttpResponseCode_ToStr(responseNoBody.code.Value())).Bytes("\r\n"_sv);
    }

    // Headers
    {
        GpHttpHeaders::SSerialize(responseNoBody.headers, aWriter);
    }

    // End of headers
    aWriter.Bytes("\r\n"_sv);

    // Body
    GpHttpBodyPayload::SP bodySP = aHttpResponse.iBody;

    if (bodySP.IsNotNULL())
    {
        const GpHttpBodyPayload& body = bodySP.V();

        // TODO: implement multipart
        THROW_COND_GP
        (
            body.Type() == GpHttpBodyPayloadType::FIXED_SIZE,
            "Only FIXED_SIZE supported at the moment"
        );

        const GpHttpBodyPayloadFixed&   bodyFixedSize               = static_cast<const GpHttpBodyPayloadFixed&>(body);
        GpSpanByteR                     bodyData                    = bodyFixedSize.Data();
        const ssize_t                   bodySize                    = NumOps::SConvert<ssize_t>(bodyData.Count());
        const ssize_t                   headersSize                 = NumOps::SConvert<ssize_t>(aWriter.TotalWrite());
        const ssize_t                   totalSize                   = NumOps::SAdd<ssize_t>(bodySize, headersSize);
        const ssize_t                   sizeLeftFromOneTcpPacket    = NumOps::SSub<ssize_t>(ssize_t{1400}, totalSize);

        if (sizeLeftFromOneTcpPacket >= 0)
        {
            aWriter.Bytes(bodyData);
            return SerializeRes::WRITE_HEADERS_AND_BODY;
        }
    }

    return SerializeRes::WRITE_HEADERS_ONLY;
}

}// namespace GPlatform

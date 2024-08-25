#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpResponse.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadFixed.hpp>

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
    //
    GpHttpBodyPayloadFixed::SP payloadSP = MakeSP<GpHttpBodyPayloadFixed>
    (
        GpBytesArrayUtils::SMake<GpBytesArray>(std::string_view(aHttpEx.what()))
    );

    //
    GpHttpResponseNoBodyDesc rsDesc;

    rsDesc.code         = aHttpEx.Code();
    rsDesc.http_version = aHttpVersion;
    rsDesc.headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetCacheControl(GpHttpCacheControl::NO_STORE)
        .SetContentLength(payloadSP->Data().Count());

    return MakeSP<GpHttpResponse>
    (
        std::move(rsDesc),
        std::move(payloadSP)
    );
}

GpHttpResponse::SP  GpHttpResponse::SFromException
(
    const GpException&          aEx,
    const GpHttpVersion::EnumT  aHttpVersion
)
{
    //
    GpHttpBodyPayloadFixed::SP payloadSP = MakeSP<GpHttpBodyPayloadFixed>
    (
        GpBytesArrayUtils::SMake<GpBytesArray>(std::string_view(aEx.what()))
    );

    //
    GpHttpResponseNoBodyDesc rsDesc;

    rsDesc.code         = GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
    rsDesc.http_version = aHttpVersion;
    rsDesc.headers
        .SetContentType(GpHttpContentType::TEXT_PLAIN, GpHttpCharset::UTF_8)
        .SetCacheControl(GpHttpCacheControl::NO_STORE)
        .SetContentLength(payloadSP->Data().Count());

    return MakeSP<GpHttpResponse>
    (
        std::move(rsDesc),
        std::move(payloadSP)
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
        MakeSP<GpHttpBodyPayloadFixed>
        (
            GpBytesArrayUtils::SMake<GpBytesArray>(std::string_view(aEx.what()))
        )
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
    if (!responseNoBody.headers.Empty())
    {
        GpHttpHeaders::SSerialize(responseNoBody.headers, aWriter);

        // End of headers
        aWriter.Bytes("\r\n"_sv);
    } else
    {
        // End of headers
        aWriter.Bytes("\r\n\r\n"_sv);
    }

    // Body
    GpHttpBodyPayload::SP bodySP = aHttpResponse.iBody;

    if (bodySP.IsNULL())
    {
        return SerializeRes::WRITE_HEADERS_AND_BODY;
    }

    // ------------ Try to write body ------------
    const GpHttpBodyPayload& body = bodySP.Vn();

    // TODO: implement multipart
    THROW_COND_GP
    (
        body.Type() == GpHttpBodyPayloadType::FIXED_SIZE,
        "Only FIXED_SIZE supported at the moment"
    );

    const GpHttpBodyPayloadFixed&   bodyFixedPayload            = static_cast<const GpHttpBodyPayloadFixed&>(body);
    GpSpanByteR                     bodyData                    = bodyFixedPayload.Data();
    const ssize_t                   bodySize                    = NumOps::SConvert<ssize_t>(bodyData.Count());
    const ssize_t                   headersSize                 = NumOps::SConvert<ssize_t>(aWriter.TotalWrite());
    const ssize_t                   totalSize                   = NumOps::SAdd<ssize_t>(bodySize, headersSize);
    const ssize_t                   sizeLeftFromOneTcpPacket    = NumOps::SSub<ssize_t>(ssize_t{1400}, totalSize); // TODO: move 1400 to config

    if (sizeLeftFromOneTcpPacket >= 0)
    {
        // Write all body data
        aWriter.Bytes(bodyData);
        return SerializeRes::WRITE_HEADERS_AND_BODY;
    } else
    {
        return SerializeRes::WRITE_HEADERS_ONLY;
    }
}

}// namespace GPlatform

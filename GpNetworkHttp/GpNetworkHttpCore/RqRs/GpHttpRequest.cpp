#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpRequest.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadFixed.hpp>

namespace GPlatform {

const GpHttpRequest::RequestTypeToStrT  GpHttpRequest::sRequestTypeToStr =
{
    "GET",
    "HEAD",
    "POST",
    "PUT",
    "DELETE",
    "CONNECT",
    "OPTIONS",
    "TRACE",
    "PATCH"
};

GpHttpRequest::~GpHttpRequest (void) noexcept
{
}

GpHttpRequest::SerializeRes GpHttpRequest::SSerialize
(
    GpHttpRequest&  aHttpRequest,
    GpByteWriter&   aWriter
)
{
    GpHttpRequestNoBodyDesc& reqestNoBody = aHttpRequest.iRequestNoBody;

    // HTTP Method
    {
        const size_t idx = NumOps::SConvert<size_t>(reqestNoBody.request_type.ID());
        aWriter.Bytes(sRequestTypeToStr.at(idx));
    }

    // URL
    aWriter
        .Bytes(" "_sv)
        .Bytes(reqestNoBody.url.ToString({GpUrlPartType::PATH_QUERY_FRAGMENT}))
        .Bytes(" "_sv);

    // HTTP version
    switch (reqestNoBody.http_version.Value())
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

    aWriter.Bytes("\r\n"_sv);

    GpHttpBodyPayload::SP bodySP = aHttpRequest.iBody;

    // Headers
    {
        GpHttpHeaders& headers = reqestNoBody.headers;
        headers.SetHost(reqestNoBody.url.ToString({GpUrlPartType::AUTHORITY_HOST_AND_PORT}));
        headers.SetUserAgent("GPlatform-Network");

        if (bodySP.IsNotNULL())
        {
            headers.SetContentLength(bodySP.Vn().Size());
        }

        GpHttpHeaders::SSerialize(headers, aWriter);

        // End of headers
        aWriter.Bytes("\r\n"_sv);
    }

    // Body
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

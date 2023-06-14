#include "GpHttpResponseSerializer.hpp"

namespace GPlatform {

std::array<std::u8string, GpHttpVersion::SCount()>      GpHttpResponseSerializer::sHttpVersion =
{
    std::u8string(u8"HTTP/1.0"_sv), //HTTP_1_0
    std::u8string(u8"HTTP/1.1"_sv)  //HTTP_1_1
};

std::array<std::u8string, GpHttpResponseCode::SCount()> GpHttpResponseSerializer::sHttpResponseCode =
{
    std::u8string(u8"100 Continue"_sv),
    std::u8string(u8"101 Switching Protocols"_sv),
    std::u8string(u8"102 Processing"_sv),
    std::u8string(u8"103 Early Hints"_sv),
    std::u8string(u8"200 OK"_sv),
    std::u8string(u8"201 Created"_sv),
    std::u8string(u8"202 Accepted_"_sv),
    std::u8string(u8"203 Non Authoritative Information"_sv),
    std::u8string(u8"204 No Content"_sv),
    std::u8string(u8"205 Reset Content"_sv),
    std::u8string(u8"206 Partial Content"_sv),
    std::u8string(u8"300 Multiple Choices"_sv),
    std::u8string(u8"301 Moved Permanently"_sv),
    std::u8string(u8"302 Found"_sv),
    std::u8string(u8"303 See Other"_sv),
    std::u8string(u8"304 Not Modified"_sv),
    std::u8string(u8"305 Use Proxy"_sv),
    std::u8string(u8"306 Switch Proxy"_sv),
    std::u8string(u8"307 Temporary Redirect"_sv),
    std::u8string(u8"308 Permanent Redirect"_sv),
    std::u8string(u8"400 Bad Request"_sv),
    std::u8string(u8"401 Unauthorized"_sv),
    std::u8string(u8"402 Payment Required"_sv),
    std::u8string(u8"403 Forbidden"_sv),
    std::u8string(u8"404 Not Found"_sv),
    std::u8string(u8"405 Method Not Allowed"_sv),
    std::u8string(u8"406 Not Acceptable"_sv),
    std::u8string(u8"407 Proxy Authentication Required"_sv),
    std::u8string(u8"408 Request Timeout"_sv),
    std::u8string(u8"409 Conflict"_sv),
    std::u8string(u8"410 Gone"_sv),
    std::u8string(u8"411 Length Required"_sv),
    std::u8string(u8"412 Precondition Failed"_sv),
    std::u8string(u8"413 Payload Too Large"_sv),
    std::u8string(u8"414 Uri Too Long"_sv),
    std::u8string(u8"415 Unsupported Media Type"_sv),
    std::u8string(u8"416 Range Not Satisfiable"_sv),
    std::u8string(u8"417 Expectation Failed"_sv),
    std::u8string(u8"500 Internal Server Error"_sv),
    std::u8string(u8"501 Not Implemented"_sv),
    std::u8string(u8"502 Bad Gateway"_sv),
    std::u8string(u8"503 Service Unavailable"_sv),
    std::u8string(u8"504 Gateway Timeout"_sv),
    std::u8string(u8"505 Http Version Not Supported"_sv),
    std::u8string(u8"506 Variant Also Negotiates"_sv)
};

void    GpHttpResponseSerializer::SSerializeHeaders
(
    const GpHttpResponse&   aResponse,
    GpByteWriter&           aWriter
)
{
    SHeadersFirstLine(aResponse, aWriter);
    SHeaders(aResponse, aWriter);
}

void    GpHttpResponseSerializer::SHeadersFirstLine
(
    const GpHttpResponse&   aResponse,
    GpByteWriter&           aWriter
)
{
    const GpHttpResponse& response = aResponse;

    //Http version
    aWriter.Bytes(sHttpVersion.at(NumOps::SConvert<size_t>(response.http_version.ID())));

    //Space
    aWriter.Bytes(" "_sv);

    //Code
    aWriter.Bytes(sHttpResponseCode.at(NumOps::SConvert<size_t>(response.code.ID())));

    //Next line
    aWriter.Bytes("\r\n"_sv);
}

void    GpHttpResponseSerializer::SHeaders
(
    const GpHttpResponse&   aResponse,
    GpByteWriter&           aWriter
)
{
    const GpHttpResponse&   response    = aResponse;
    const GpHttpHeaders&    headers     = response.headers;

    for (const auto&[name, header]: headers.Headers())
    {
        aWriter.Bytes(name);
        aWriter.Bytes(u8": "_sv);
        aWriter.Bytes(StrOps::SJoin<std::u8string_view>(header.V().elements, u8";"_sv));
        aWriter.Bytes(u8"\r\n"_sv);
    }

    //Next line
    aWriter.Bytes("\r\n"_sv);
}

}//namespace GPlatform

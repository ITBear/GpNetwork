#include "GpHttpResponseSerializer.hpp"

namespace GPlatform {

std::array<std::string, GpHttpVersion::SCount()>        GpHttpResponseSerializer::sHttpVersion =
{
    std::string("HTTP/1.0"_sv), //HTTP_1_0
    std::string("HTTP/1.1"_sv)  //HTTP_1_1
};

std::array<std::string, GpHttpResponseCode::SCount()>   GpHttpResponseSerializer::sHttpResponseCode =
{
    std::string("100 Continue"_sv),
    std::string("101 Switching Protocols"_sv),
    std::string("102 Processing"_sv),
    std::string("103 Early Hints"_sv),
    std::string("200 OK"_sv),
    std::string("201 Created"_sv),
    std::string("202 Accepted_"_sv),
    std::string("203 Non Authoritative Information"_sv),
    std::string("204 No Content"_sv),
    std::string("205 Reset Content"_sv),
    std::string("206 Partial Content"_sv),
    std::string("300 Multiple Choices"_sv),
    std::string("301 Moved Permanently"_sv),
    std::string("302 Found"_sv),
    std::string("303 See Other"_sv),
    std::string("304 Not Modified"_sv),
    std::string("305 Use Proxy"_sv),
    std::string("306 Switch Proxy"_sv),
    std::string("307 Temporary Redirect"_sv),
    std::string("308 Permanent Redirect"_sv),
    std::string("400 Bad Request"_sv),
    std::string("401 Unauthorized"_sv),
    std::string("402 Payment Required"_sv),
    std::string("403 Forbidden"_sv),
    std::string("404 Not Found"_sv),
    std::string("405 Method Not Allowed"_sv),
    std::string("406 Not Acceptable"_sv),
    std::string("407 Proxy Authentication Required"_sv),
    std::string("408 Request Timeout"_sv),
    std::string("409 Conflict"_sv),
    std::string("410 Gone"_sv),
    std::string("411 Length Required"_sv),
    std::string("412 Precondition Failed"_sv),
    std::string("413 Payload Too Large"_sv),
    std::string("414 Uri Too Long"_sv),
    std::string("415 Unsupported Media Type"_sv),
    std::string("416 Range Not Satisfiable"_sv),
    std::string("417 Expectation Failed"_sv),
    std::string("500 Internal Server Error"_sv),
    std::string("501 Not Implemented"_sv),
    std::string("502 Bad Gateway"_sv),
    std::string("503 Service Unavailable"_sv),
    std::string("504 Gateway Timeout"_sv),
    std::string("505 Http Version Not Supported"_sv),
    std::string("506 Variant Also Negotiates"_sv)
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
    aWriter.Bytes(sHttpVersion.at(response.http_version.ID()));

    //Space
    aWriter.Bytes(" "_sv);

    //Code
    aWriter.Bytes(sHttpResponseCode.at(response.code.ID()));

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
        aWriter.Bytes(": "_sv);
        aWriter.Bytes(StrOps::SJoin<std::string_view>(header.V().elements, ";"_sv));
        aWriter.Bytes("\r\n"_sv);
    }

    //Next line
    aWriter.Bytes("\r\n"_sv);
}

}//namespace GPlatform

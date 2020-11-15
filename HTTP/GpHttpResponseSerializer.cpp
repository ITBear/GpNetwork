#include "GpHttpResponseSerializer.hpp"

namespace GPlatform {

GpArray<std::string, GpHttpVersion::SCount().As<size_t>()>          GpHttpResponseSerializer::sHttpVersion =
{
    std::string("HTTP/1.0"_sv), //HTTP_1_0
    std::string("HTTP/1.1"_sv)  //HTTP_1_1
};

GpArray<std::string, GpHttpResponseCode::SCount().As<size_t>()>     GpHttpResponseSerializer::sHttpResponseCode =
{
    std::string("100 Continue"_sv),                 //CONTINUE_100
    std::string("101 Switching Protocols"_sv),      //SWITCHING_PROTOCOLS_101
    std::string("200 OK"_sv),                       //OK_200
    std::string("400 Bad Request"_sv),              //BAD_REQUEST_400
    std::string("401 Unauthorized"_sv),             //UNAUTHORIZED_401
    std::string("402 Payment Required"_sv),         //PAYMENT_REQUIRED_402
    std::string("403 Forbidden"_sv),                //FORBIDDEN_403
    std::string("404 Not Found"_sv),                //NOT_FOUND_404
    std::string("405 Method Not Allowed"_sv),       //METHOD_NOT_ALLOWED_405
    std::string("406 Not Acceptable"_sv),           //NOT_ACCEPTABLE_406
    std::string("500 Internal Server Error"_sv)     //INTERNAL_SERVER_ERROR_500
};

void    GpHttpResponseSerializer::SSerializeHeaders (const GpHttpResponse&  aResponse,
                                                     GpByteWriter&          aWriter)
{
    SHeadersFirstLine(aResponse, aWriter);
    SHeaders(aResponse, aWriter);
}

void    GpHttpResponseSerializer::SHeadersFirstLine (const GpHttpResponse&  aResponse,
                                                     GpByteWriter&          aWriter)
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

void    GpHttpResponseSerializer::SHeaders (const GpHttpResponse&   aResponse,
                                            GpByteWriter&           aWriter)
{
    const GpHttpResponse&   response    = aResponse;
    const GpBytesArray&     body        = response.body;
    const GpHttpHeaders&    headers     = response.headers;

    for (const auto&[name, header]: headers.headers)
    {
        aWriter.Bytes(name);
        aWriter.Bytes(": "_sv);
        aWriter.Bytes(GpStringOps::SJoin<std::string_view>(header.VC().elements, ";"_sv));
        aWriter.Bytes("\r\n"_sv);
    }

    //Next line
    aWriter.Bytes("\r\n"_sv);
}

}//namespace GPlatform

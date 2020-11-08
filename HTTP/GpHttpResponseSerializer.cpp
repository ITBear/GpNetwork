#include "GpHttpResponseSerializer.hpp"

namespace GPlatform {

GpArray<std::string, GpHttpVersion::SCount().As<size_t>()>          GpHttpResponseSerializer::sHttpVersion =
{
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

GpArray<std::string, GpHttpContentType::SCount().As<size_t>()>      GpHttpResponseSerializer::sHttpContentType =
{
    std::string("text/plain"_sv),       //TEXT_PLAIN
    std::string("application/json"_sv), //APPLICATION_JSON
    std::string("application/xml"_sv)   //APPLICATION_XML
};

GpArray<std::string, GpHttpCharset::SCount().As<size_t>()>          GpHttpResponseSerializer::sHttpCharset =
{
    std::string(""_sv),                 //NOT_SET
    std::string("charset=utf-8"_sv),    //UTF_8
};

GpArray<std::string, GpHttpConnectionFlag::SCount().As<size_t>()>   GpHttpResponseSerializer::sHttpConnectionFlag =
{
    std::string("close"_sv),            //CLOSE
    std::string("keep-alive"_sv),       //KEEP_ALIVE
};

GpArray<std::string, GpHttpCacheControl::SCount().As<size_t>()>     GpHttpResponseSerializer::sHttpCacheControl =
{
    std::string("no-store"_sv),         //NO_STORE
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
    aWriter.Bytes(sHttpVersion.at(size_t(response.HttpVersion())));

    //Space
    aWriter.Bytes(" "_sv);

    //Code
    aWriter.Bytes(sHttpResponseCode.at(size_t(response.Code())));

    //Next line
    aWriter.Bytes("\r\n"_sv);
}

void    GpHttpResponseSerializer::SHeaders (const GpHttpResponse&   aResponse,
                                            GpByteWriter&           aWriter)
{
    const GpHttpResponse&   response    = aResponse;
    const GpBytesArray&     body        = response.Body();

    if (body.size() > 0)
    {
        //Content type
        {
            aWriter.Bytes("Content-Type: "_sv);
            aWriter.Bytes(sHttpContentType.at(size_t(response.ContentType())));
            if (response.Charset() != GpHttpCharset::NOT_SET)
            {
                aWriter.Bytes(";"_sv);
                aWriter.Bytes(sHttpCharset.at(size_t(response.Charset())));
            }
            aWriter.Bytes("\r\n"_sv);
        }

        //Content length
        {
            aWriter.Bytes("Content-Length: "_sv);
            aWriter.Bytes(GpStringOps::SFromUI64(NumOps::SConvert<u_int_64>(body.size())));
            aWriter.Bytes("\r\n"_sv);
        }
    }

    //Connection
    {
        aWriter.Bytes("Connection: "_sv);
        aWriter.Bytes(sHttpConnectionFlag.at(size_t(response.ConnectionFlag())));
        aWriter.Bytes("\r\n"_sv);
    }

    //Cache control
    {
        aWriter.Bytes("Cache-Control: "_sv);
        aWriter.Bytes(sHttpCacheControl.at(size_t(response.CacheControl())));
        aWriter.Bytes("\r\n"_sv);
    }
}

}//namespace GPlatform

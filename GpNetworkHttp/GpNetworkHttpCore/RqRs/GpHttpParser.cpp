#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpParser.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Exceptions/GpHttpException.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadFixed.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>

namespace GPlatform {

GpHttpParser::GpHttpParser (void) noexcept
{
}

GpHttpParser::~GpHttpParser (void) noexcept
{
}

void    GpHttpParser::InitCycle (const Mode aMode)
{
    iMode = aMode;
    llhttp_settings_init(&iHttpParserSettings);

    iHttpParserSettings.on_status           = SHttp_OnStatus;
    iHttpParserSettings.on_url              = SHttp_OnURL;
    iHttpParserSettings.on_header_field     = SHttp_OnHeaderField;
    iHttpParserSettings.on_header_value     = SHttp_OnHeaderValue;
    iHttpParserSettings.on_headers_complete = SHttp_OnHeadersComplete;
    iHttpParserSettings.on_body             = SHttp_OnBody;
    iHttpParserSettings.on_message_begin    = SHttp_OnMessageBegin;
    iHttpParserSettings.on_message_complete = SHttp_OnMessageComplete;

    llhttp_init
    (
        &iLLHttpParser,
        iMode == Mode::HTTP_REQUEST ? HTTP_REQUEST : HTTP_RESPONSE,
        &iHttpParserSettings
    );

    iHttpParserSettings.iHttpParser = this;
    iHttpParserCurrentHeaderName.clear();
    iIsDone = false;
}

void    GpHttpParser::FinishCycle (void)
{
    llhttp_finish(&iLLHttpParser);
    iIsDone = false;

    iRqNoBody.Clear();
    iRsNoBody.Clear();
    iBodyPayloadSP.Clear();
}

bool    GpHttpParser::ParseNext (GpSpanByteR aNextDataBlock)
{
    if (aNextDataBlock.Empty())
    {
        return iIsDone;
    }

    const enum llhttp_errno httpParseRes = llhttp_execute
    (
        &iLLHttpParser,
        reinterpret_cast<const char*>(std::data(aNextDataBlock)),
        aNextDataBlock.Count()
    );

    THROW_COND_HTTP
    (
        httpParseRes == HPE_OK,
        GpHttpResponseCode::BAD_REQUEST_400,
        [&]()
        {
            return std::string(llhttp_errno_name(httpParseRes));
        }
    );

    return iIsDone;
}

int GpHttpParser::SHttp_OnStatus
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SCastToHttpSettings(aHttp).iHttpParser->Http_OnStatus
    (
        aHttp,
        std::string_view(aData, aLength)
    );  
}

int GpHttpParser::Http_OnStatus
(
    llhttp_t*                           aParser,
    [[maybe_unused]] std::string_view   aValue
)
{
    if (iMode == Mode::HTTP_RESPONSE)
    {
        iRsNoBody.code = GpHttpResponseCode::SFromID(aParser->status_code);
    }

    return HPE_OK;
}

int GpHttpParser::SHttp_OnURL
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SCastToHttpSettings(aHttp).iHttpParser->Http_OnURL
    (
        std::string_view(aData, aLength)
    );
}

int GpHttpParser::Http_OnURL (std::string_view aValue)
{
    if (iMode == Mode::HTTP_REQUEST)
    {
        try
        {
            iRqNoBody.url = GpUrl::SFromString(aValue);
        } catch (const GpException& e)
        {
            THROW_HTTP
            (
                GpHttpResponseCode::BAD_REQUEST_400,
                fmt::format
                (
                    "Wrong URL '{}'. {}",
                    aValue,
                    e.what()
                )
            );
        }
    }

    return HPE_OK;
}

int GpHttpParser::SHttp_OnHeaderField
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SCastToHttpSettings(aHttp).iHttpParser->Http_OnHeaderField
    (
        std::string_view(aData, aLength)
    );
}

int GpHttpParser::Http_OnHeaderField (std::string_view aValue)
{
    iHttpParserCurrentHeaderName = GpUTF::SToLower(aValue);

    return HPE_OK;
}

int GpHttpParser::SHttp_OnHeaderValue
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SCastToHttpSettings(aHttp).iHttpParser->Http_OnHeaderValue
    (
        std::string_view(aData, aLength)
    );
}

int GpHttpParser::Http_OnHeaderValue (std::string_view aValue)
{
    if (iMode == Mode::HTTP_REQUEST)
    {
        iRqNoBody.headers.Add
        (
            iHttpParserCurrentHeaderName,
            std::string{aValue}
        );
    } else
    {
        iRsNoBody.headers.Add
        (
            iHttpParserCurrentHeaderName,
            std::string{aValue}
        );
    }

    return HPE_OK;
}

int GpHttpParser::SHttp_OnHeadersComplete (llhttp_t* aHttp)
{
    return SCastToHttpSettings(aHttp).iHttpParser->Http_OnHeadersComplete(aHttp);
}

int GpHttpParser::Http_OnHeadersComplete (llhttp_t* aHttp)
{
    // Check if there no body
    if (aHttp->content_length == 0)
    {
        // No body
        return 1;
    }  else
    {
        // TODO: add check for max content_length value to config
        THROW_COND_HTTP
        (
            aHttp->content_length <= 1024*1024,
            GpHttpResponseCode::PAYLOAD_TOO_LARGE_413
        );

        return HPE_OK;
    }
}

int GpHttpParser::SHttp_OnBody
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SCastToHttpSettings(aHttp).iHttpParser->Http_OnBody(GpSpanByteR(aData, aLength));
}

int GpHttpParser::Http_OnBody (GpSpanByteR aValue)
{
    // TODO: implement multipart
    if (iBodyPayloadSP.IsNULL())
    {
        iBodyPayloadSP = MakeSP<GpHttpBodyPayloadFixed>();
    }

    // Read new data
    {
        GpBytesArray& bodyPayloadFixed = static_cast<GpHttpBodyPayloadFixed&>(iBodyPayloadSP.Vn()).DataStorage();

        const size_t currentSize            = std::size(bodyPayloadFixed);
        const size_t totalSizeWithNewData   = NumOps::SAdd<size_t>(currentSize, aValue.Count());
        bodyPayloadFixed.resize(totalSizeWithNewData);

        GpByteWriterStorageByteArray    writerStorage(bodyPayloadFixed);
        GpByteWriter                    writer(writerStorage);

        writer.SubspanThenOffsetAdd(currentSize);
        writer.Bytes(aValue);
    }

    return HPE_OK;
}

int GpHttpParser::SHttp_OnMessageBegin (llhttp_t* aHttp)
{
    return SCastToHttpSettings(aHttp).iHttpParser->Http_OnMessageBegin(aHttp);
}

int GpHttpParser::Http_OnMessageBegin ([[maybe_unused]] llhttp_t* aHttp)
{
    return HPE_OK;
}

int GpHttpParser::SHttp_OnMessageComplete (llhttp_t* aHttp)
{
    return SCastToHttpSettings(aHttp).iHttpParser->Http_OnMessageComplete(aHttp);
}

int GpHttpParser::Http_OnMessageComplete (llhttp_t* aHttp)
{
    // Version
    if (iMode == Mode::HTTP_REQUEST)
    {
        iRqNoBody.SetHttpVersion(aHttp->http_major, aHttp->http_minor);
    } else
    {
        iRsNoBody.SetHttpVersion(aHttp->http_major, aHttp->http_minor);
    }

    if (iMode == Mode::HTTP_REQUEST)
    {
        // Request type
        GpHttpRequestType::EnumT httpRequestType;

        switch (aHttp->method)
        {
            case HTTP_GET:      httpRequestType = GpHttpRequestType::HTTP_GET;      break;
            case HTTP_HEAD:     httpRequestType = GpHttpRequestType::HTTP_HEAD;     break;
            case HTTP_POST:     httpRequestType = GpHttpRequestType::HTTP_POST;     break;
            case HTTP_PUT:      httpRequestType = GpHttpRequestType::HTTP_PUT;      break;
            case HTTP_DELETE:   httpRequestType = GpHttpRequestType::HTTP_DELETE;   break;
            case HTTP_CONNECT:  httpRequestType = GpHttpRequestType::HTTP_CONNECT;  break;
            case HTTP_OPTIONS:  httpRequestType = GpHttpRequestType::HTTP_OPTIONS;  break;
            case HTTP_TRACE:    httpRequestType = GpHttpRequestType::HTTP_TRACE;    break;
            case HTTP_PATCH:    httpRequestType = GpHttpRequestType::HTTP_PATCH;    break;
            default:
            {
                THROW_HTTP(GpHttpResponseCode::BAD_REQUEST_400, "Unsupported HTTP method"_sv);
            }
        }

        iRqNoBody.request_type = httpRequestType;

        // URL
        iRqNoBody.url.SetFromHeaders(iRqNoBody.headers);
    }

    iIsDone = true;

    return HPE_OK;
}

}// namespace GPlatform

#include "GpHttpRequestSocketTask.hpp"

#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReaderStorage.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadFixed.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpRequest.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpResponse.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpHttpRequestSocketTask::GpHttpRequestSocketTask
(
    GpSocket::SP                aSocket,
    GpHttpRouter::SP            aRouter,
    const GpIOEventPollerIdx    aIOEventPollerIdx
) noexcept:
GpSingleSocketTask(std::move(aSocket), aIOEventPollerIdx),
iRouter(aRouter)
{
}

GpHttpRequestSocketTask::~GpHttpRequestSocketTask (void) noexcept
{
}

void    GpHttpRequestSocketTask::OnReadyToRead (GpSocket& aSocket)
{
    try
    {
        if (iProcessState == ProcessStateT::WAIT_FOR_RQ)
        {
            InitCycle();
        }

        if (iProcessState == ProcessStateT::PARSE_RQ)
        {
            ReadNextAndParseRq(aSocket);
        }

        if (iProcessState == ProcessStateT::RQ_READY)
        {
            // Try to find handler
            GpHttpRequestHandler::SP requestHandlerSP = iRouter->FindHandler(iRequestNoBody);

            iProcessState = ProcessStateT::RS_IN_PROGRESS;
            {
                iHttpResponseSP = requestHandlerSP->ProcessRequest
                (
                    MakeSP<GpHttpRequest>
                    (
                        std::move(iRequestNoBody),
                        std::move(iRequestBodyPayloadSP)
                    )
                );
            }
            iProcessState = ProcessStateT::WRITE_RS;

            WriteRsToSocket(aSocket);
        }

        if (   (iProcessState == ProcessStateT::RQ_READY)
            || (iProcessState == ProcessStateT::RS_IN_PROGRESS)
            || (iProcessState == ProcessStateT::WRITE_RS))
        {
            GpBytesArray buff;
            const size_t readFromSocketSize = SReadFromSocket(aSocket, buff);

            THROW_COND_GP
            (
                readFromSocketSize == 0,
                "Data income after RQ was full read"
            );
        }
    } catch (const GpHttpException& httpEx)
    {
        iHttpResponseSP = GpHttpResponse::SFromException(httpEx, iRequestNoBody.http_version);
        iProcessState   = ProcessStateT::WRITE_RS;

        WriteRsToSocket(aSocket);
    }
}

void    GpHttpRequestSocketTask::OnReadyToWrite (GpSocket& aSocket)
{
    WriteRsToSocket(aSocket);
}

void    GpHttpRequestSocketTask::OnClosed ([[maybe_unused]] GpSocket& aSocket)
{
    // NOP
}

void    GpHttpRequestSocketTask::OnError (GpSocket& aSocket)
{
    LOG_ERROR
    (
        fmt::format
        (
            "HTTP TCP socket error. Socket id {}",
            aSocket.Id()
        )
    );
}

void    GpHttpRequestSocketTask::ProcessOtherMessages (GpAny& aMessage)
{
    LOG_ERROR
    (
        fmt::format
        (
            "Get not socket message {}",
            aMessage.TypeInfo().name()
        )
    );
}

void    GpHttpRequestSocketTask::ReadNextAndParseRq (GpSocket& aSocket)
{
    // Read from socket
    const size_t readFromSocketSize = SReadFromSocket(aSocket, iSocketTmpBuffer);

    if (readFromSocketSize == 0)
    {
        return;
    }

    const enum llhttp_errno httpParseRes = llhttp_execute
    (
        &iHttpParser,
        reinterpret_cast<const char*>(std::data(iSocketTmpBuffer)),
        readFromSocketSize
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
}

int GpHttpRequestSocketTask::SHttp_OnURL
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SHttpSettings(aHttp).iHttpRequestSocketTask->Http_OnURL
    (
        std::string_view(aData, aLength)
    );
}

int GpHttpRequestSocketTask::Http_OnURL (std::string_view aValue)
{
    try
    {
        iRequestNoBody.url = GpUrl::SFromString(aValue);
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

    return HPE_OK;
}

int GpHttpRequestSocketTask::SHttp_OnHeaderField
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SHttpSettings(aHttp).iHttpRequestSocketTask->Http_OnHeaderField
    (
        std::string_view(aData, aLength)
    );
}

int GpHttpRequestSocketTask::Http_OnHeaderField (std::string_view aValue)
{
    iHttpParserCurrentHeaderName = aValue;

    return HPE_OK;
}

int GpHttpRequestSocketTask::SHttp_OnHeaderValue
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SHttpSettings(aHttp).iHttpRequestSocketTask->Http_OnHeaderValue
    (
        std::string_view(aData, aLength)
    );
}

int GpHttpRequestSocketTask::Http_OnHeaderValue (std::string_view aValue)
{
    iRequestNoBody.headers.Add
    (
        iHttpParserCurrentHeaderName,
        std::string{aValue}
    );

    return HPE_OK;
}

int GpHttpRequestSocketTask::SHttp_OnHeadersComplete (llhttp_t* aHttp)
{
    return SHttpSettings(aHttp).iHttpRequestSocketTask->Http_OnHeadersComplete(aHttp);
}

int GpHttpRequestSocketTask::Http_OnHeadersComplete (llhttp_t* aHttp)
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

int GpHttpRequestSocketTask::SHttp_OnBody
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SHttpSettings(aHttp).iHttpRequestSocketTask->Http_OnBody(GpSpanByteR(aData, aLength));
}

int GpHttpRequestSocketTask::Http_OnBody (GpSpanByteR aValue)
{
    // TODO: implement multipart
    if (iRequestBodyPayloadSP.IsNULL())
    {
        iRequestBodyPayloadSP = MakeSP<GpHttpBodyPayloadFixed>();
    }

    // Read new data
    {
        GpBytesArray& rqBodyPayloadFixed = static_cast<GpHttpBodyPayloadFixed&>(iRequestBodyPayloadSP.Vn()).DataStorage();

        const size_t currentSize            = std::size(rqBodyPayloadFixed);
        const size_t totalSizeWithNewData   = NumOps::SAdd<size_t>(currentSize, aValue.Count());
        rqBodyPayloadFixed.resize(totalSizeWithNewData);

        GpByteWriterStorageByteArray    writerStorage(rqBodyPayloadFixed);
        GpByteWriter                    writer(writerStorage);

        writer.OffsetAdd(currentSize);
        writer.Bytes(aValue);
    }

    return HPE_OK;
}

int GpHttpRequestSocketTask::SHttp_OnMessageBegin (llhttp_t* aHttp)
{
    return SHttpSettings(aHttp).iHttpRequestSocketTask->Http_OnMessageBegin(aHttp);
}

int GpHttpRequestSocketTask::Http_OnMessageBegin ([[maybe_unused]] llhttp_t* aHttp)
{
    return HPE_OK;
}

int GpHttpRequestSocketTask::SHttp_OnMessageComplete (llhttp_t* aHttp)
{
    return SHttpSettings(aHttp).iHttpRequestSocketTask->Http_OnMessageComplete(aHttp);
}

int GpHttpRequestSocketTask::Http_OnMessageComplete (llhttp_t* aHttp)
{
    // Version
    {
        iRequestNoBody.SetHttpVersion(aHttp->http_major, aHttp->http_minor);
    }

    // Request type
    {
        GpHttpRequestType::EnumT httpRequestType;

        switch (aHttp->method)
        {
            case HTTP_GET:      httpRequestType = GpHttpRequestType::GET;       break;
            case HTTP_HEAD:     httpRequestType = GpHttpRequestType::HEAD;      break;
            case HTTP_POST:     httpRequestType = GpHttpRequestType::POST;      break;
            case HTTP_PUT:      httpRequestType = GpHttpRequestType::PUT;       break;
            case HTTP_DELETE:   httpRequestType = GpHttpRequestType::DELETE;    break;
            case HTTP_CONNECT:  httpRequestType = GpHttpRequestType::CONNECT;   break;
            case HTTP_OPTIONS:  httpRequestType = GpHttpRequestType::OPTIONS;   break;
            case HTTP_TRACE:    httpRequestType = GpHttpRequestType::TRACE;     break;
            case HTTP_PATCH:    httpRequestType = GpHttpRequestType::PATCH;     break;
            default:
            {
                THROW_HTTP(GpHttpResponseCode::BAD_REQUEST_400, "Unsupported HTTP method"_sv);
            }
        }

        iRequestNoBody.request_type = httpRequestType;
    }

    // URL authority
    {
        iRequestNoBody.url.Authority().SetFromHeaders(iRequestNoBody.headers);
    }

    iProcessState = ProcessStateT::RQ_READY;

    return HPE_OK;
}

void    GpHttpRequestSocketTask::InitCycle (void)
{
    // Init lHTTP parser
    {
        llhttp_settings_init(&iHttpParserSettings);

        iHttpParserSettings.on_url              = SHttp_OnURL;
        iHttpParserSettings.on_header_field     = SHttp_OnHeaderField;
        iHttpParserSettings.on_header_value     = SHttp_OnHeaderValue;
        iHttpParserSettings.on_headers_complete = SHttp_OnHeadersComplete;
        iHttpParserSettings.on_body             = SHttp_OnBody;
        iHttpParserSettings.on_message_begin    = SHttp_OnMessageBegin;
        iHttpParserSettings.on_message_complete = SHttp_OnMessageComplete;

        llhttp_init(&iHttpParser, HTTP_REQUEST, &iHttpParserSettings);
        iHttpParserSettings.iHttpRequestSocketTask = this;

        iHttpParserCurrentHeaderName.clear();
    }

    iSocketTmpBuffer.resize(4096); // TODO: move to config
    iProcessState = ProcessStateT::PARSE_RQ;
}

void    GpHttpRequestSocketTask::FinishCycle (void)
{
    if (iProcessState == ProcessStateT::WAIT_FOR_RQ)
    {
        return;
    }

    // Clear parser
    {
        llhttp_finish(&iHttpParser);
        iHttpParserCurrentHeaderName.clear();
    }

    // Clear RQ
    iRequestNoBody.Clear();
    iRequestBodyPayloadSP.Clear();
    iProcessState = ProcessStateT::WAIT_FOR_RQ;

    // Clear RS
    iHttpResponseSP.Clear();
    iRsWriteState = RsWriteStateT::WRITE_HEADERS_S0;
}

size_t  GpHttpRequestSocketTask::SReadFromSocket
(
    GpSocket&       aSocket,
    GpBytesArray&   aBufferOut
)
{
    GpByteWriterStorageByteArray    writerStorage(aBufferOut);
    GpByteWriter                    writer(writerStorage);

    GpSocketTCP& socketTcp = static_cast<GpSocketTCP&>(aSocket);
    return socketTcp.Read(writer);
}

void    GpHttpRequestSocketTask::WriteRsToSocket (GpSocket& aSocket)
{
    if (iProcessState != ProcessStateT::WRITE_RS)
    {
        return;
    }

    // Write headers
    {
        // Serialize headers
        if (iRsWriteState == RsWriteStateT::WRITE_HEADERS_S0)
        {
            // Serialize RS
            GpByteWriterStorageByteArray        writerStorage(iSocketTmpBuffer);
            GpByteWriter                        writer(writerStorage);
            const GpHttpResponse::SerializeRes  serializeRes = GpHttpResponse::SSerialize(iHttpResponseSP.V(), writer);
            iRsBytesToWriteTotal    = writer.TotalWrite();
            iRsBytesWrited          = 0;
            iRsWriteState           = RsWriteStateT::WRITE_HEADERS_S1;

            if (serializeRes == GpHttpResponse::SerializeRes::WRITE_HEADERS_AND_BODY)
            {
                // Write headers and body with single buffer
                iHttpResponseSP.Clear();
            }
        }

        // Write headers
        if (iRsWriteState == RsWriteStateT::WRITE_HEADERS_S1)
        {
            GpByteReaderStorage iRsReaderStorage({std::data(iSocketTmpBuffer), iRsBytesToWriteTotal});
            GpByteReader        iRsReader(iRsReaderStorage);
            iRsReader.OffsetAdd(iRsBytesWrited);

            GpSocketTCP& socketTcp  = static_cast<GpSocketTCP&>(aSocket);
            const size_t writeSize  = socketTcp.Write(iRsReader);
            iRsBytesWrited += writeSize;

            if (iRsBytesWrited >= iRsBytesToWriteTotal)
            {
                iRsWriteState = RsWriteStateT::WRITE_BODY_S0;
            }
        }
    }// Write headers

    // Write body
    {
        if (iRsWriteState == RsWriteStateT::WRITE_BODY_S0)
        {
            if (iHttpResponseSP.IsNULL())
            {
                iRsWriteState = RsWriteStateT::WRITE_DONE_S0;
            } else
            {
                iRsWriteState           = RsWriteStateT::WRITE_BODY_S1;
                iRsBytesToWriteTotal    = iHttpResponseSP->iBody->Size();
                iRsBytesWrited          = 0;
            }
        }

        if (iRsWriteState == RsWriteStateT::WRITE_BODY_S1)
        {
            // TODO: implement multipart
            const GpSpanByteR rqBodyPayloadFixed = static_cast<const GpHttpBodyPayloadFixed&>(iHttpResponseSP->iBody.V()).Data();

            //-------
            GpByteReaderStorage iRsReaderStorage(rqBodyPayloadFixed);
            GpByteReader        iRsReader(iRsReaderStorage);
            iRsReader.OffsetAdd(iRsBytesWrited);

            GpSocketTCP& socketTcp  = static_cast<GpSocketTCP&>(aSocket);
            const size_t writeSize  = socketTcp.Write(iRsReader);
            iRsBytesWrited += writeSize;

            if (iRsBytesWrited >= rqBodyPayloadFixed.Count())
            {
                iRsWriteState = RsWriteStateT::WRITE_BODY_S0;
            }
        }
    }// Write body

    // Finish
    if (iRsWriteState == RsWriteStateT::WRITE_DONE_S0)
    {
        iRsWriteState = RsWriteStateT::WRITE_DONE_S1;
        FinishCycle();
    }
}

}// namespace GPlatform

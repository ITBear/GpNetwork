#include "GpHttpServerBasicSocketTask.hpp"
#include "../GpNetworkHttpCore/RqRs/GpHttpRequestTask.hpp"
#include "../GpNetworkHttpCore/RqRs/GpHttpResponseSerializer.hpp"

namespace GPlatform {

//https://llhttp.org/
//https://pypi.org/project/llhttp/

GpHttpServerBasicSocketTask::~GpHttpServerBasicSocketTask (void) noexcept
{
}

GpTaskDoRes GpHttpServerBasicSocketTask::OnStep (EventOptRefT aEvent)
{
    if (aEvent.has_value())
    {
        const GpEvent&  event       = aEvent.value().get();
        const GpUUID&   eventType   = event.Type();

        if (eventType == GpIOEvent::SType())
        {
            return GpSocketTask::OnStep(aEvent);
        } else if (eventType == GpHttpRequestDoneEvent::SType())
        {
            return ProcessDoneEvent(static_cast<const GpHttpRequestDoneEvent&>(event));
        }
    }

    return GpTaskDoRes::WAITING;
}

GpTaskDoRes GpHttpServerBasicSocketTask::OnSockReadyToRead (GpSocket& aSocket)
{
    try
    {
        if (iState == StateT::WAIT_FOR_RQ)
        {
            InitRqRsCycle();
        }

        if (iState == StateT::PARSE_RQ)
        {
            ParseHttp(aSocket);
        }

        if (iState == StateT::PROCESS_RQ_READY_TO)
        {
            //Start request process task
            GpHttpRequestTask::SP requestTask = MakeSP<GpHttpRequestTask>
            (
                Name() + u8": http socket task"_sv,
                iRq,
                iRequestHandlerFactory.V().NewInstance(),
                *this
            );

            std::ignore = GpTaskScheduler::S().NewToReadyDepend(std::move(requestTask));
            iState = StateT::PROCESS_RQ_IN_PROGRESS;
            return GpTaskDoRes::WAITING;
        }

        if (   (iState == StateT::PROCESS_RQ_DONE_WRITE_RS)
            || (iState == StateT::PROCESS_RQ_IN_PROGRESS))
        {
            //Data income while write RS
            GpBytesArray                    data;
            GpByteWriterStorageByteArray    dataStorage(data);
            GpByteWriter                    writer(dataStorage);

            aSocket.Read(writer);
            writer.ShrinkToFit();

            if (data.size() > 0)
            {
                THROW_GP(u8"Data income while RS in progress '"_sv + GpSpanPtrCharR(data).AsStringViewU8() + u8"'"_sv);
            }
        }
    } catch (const GpHttpException& httpEx)
    {
        iRs     = MakeSP<GpHttpResponse>();
        iRs.V().SetFromException(httpEx);
        iState  = StateT::PROCESS_RQ_DONE_WRITE_RS;
        iRs.Vn().http_version = iRq.Vn().http_version;
        return WriteToSocket(aSocket);
    }

    return GpTaskDoRes::WAITING;
}

GpTaskDoRes GpHttpServerBasicSocketTask::OnSockReadyToWrite (GpSocket& aSocket)
{
    return WriteToSocket(aSocket);
}

void    GpHttpServerBasicSocketTask::OnSockClosed (GpSocket& /*aSocket*/)
{
    ClearRqRsCycle();
}

void    GpHttpServerBasicSocketTask::OnSockError (GpSocket& /*aSocket*/)
{
    ClearRqRsCycle();
}

GpTaskDoRes GpHttpServerBasicSocketTask::ProcessDoneEvent (const GpHttpRequestDoneEvent& aRequestDoneEvent)
{
    THROW_COND_GP
    (
        iState == StateT::PROCESS_RQ_IN_PROGRESS,
        "State expected to be 'PROCESS_RQ_IN_PROGRESS'"_sv
    );

    iRs     = aRequestDoneEvent.Response();
    iState  = StateT::PROCESS_RQ_DONE_WRITE_RS;
    iRs.V().http_version = iRq.V().http_version;
    return WriteToSocket(Socket());
}

GpTaskDoRes GpHttpServerBasicSocketTask::WriteToSocket (GpSocket& aSocket)
{
    if (iState != StateT::PROCESS_RQ_DONE_WRITE_RS)
    {
        return GpTaskDoRes::WAITING;
    }

    //Serialize rs headers
    if (iRsHeadersData.size() == 0)
    {
        const GpBytesArray& body = iRs.V().body;
        iRs.V().headers.SetContentLength(body.size());
        iRs.V().headers.SetConnection(GpHttpConnectionFlag::CLOSE);

        iRsHeadersData.reserve(1024);//TODO: move to config
        GpByteWriterStorageByteArray    rsHeadersDataStorage(iRsHeadersData);
        GpByteWriter                    rsHeadersWriter(rsHeadersDataStorage);
        GpHttpResponseSerializer::SSerializeHeaders(iRs.V(), rsHeadersWriter);

        iRsReaderStorage = GpByteReaderStorage(iRsHeadersData);
    }

    //Write headers/body
    if (iRsReaderStorage.has_value())
    {
        GpByteReaderStorage& rsDataReaderStorage = iRsReaderStorage.value();

        if (rsDataReaderStorage.SizeLeft() > 0)
        {
            GpByteReader    reader(rsDataReaderStorage);
            const size_t    bytesWrite = aSocket.Write(reader);

            if (bytesWrite == 0)//No space left in socket buffer
            {
                return GpTaskDoRes::WAITING;
            } else if (rsDataReaderStorage.SizeLeft() > 0)//No space left in socket buffer
            {
                return GpTaskDoRes::WAITING;
            } else//All data has been write to socket
            {
                if (iRsWriteState == RsWriteStateT::WRITE_HEADERS)
                {
                    //Check if there are body
                    const GpBytesArray& body = iRs.V().body;

                    if (body.size() > 0)
                    {
                        iRsWriteState       = RsWriteStateT::WRITE_BODY;
                        iRsReaderStorage    = GpByteReaderStorage(GpSpanPtrByteR(body.data(), body.size()));

                        return WriteToSocket(aSocket);
                    }
                } else if (iRsWriteState == RsWriteStateT::WRITE_BODY)
                {
                    //const GpUUID taskGuid = GPlatform::GpTaskFiber::SGuid();
                    //NOP, go to finish
                }
            }
        }
    }

    //Finish
    ClearRqRsCycle();
    return GpTaskDoRes::DONE;//TODO: add timeout
}

void    GpHttpServerBasicSocketTask::InitRqRsCycle (void)
{
    //Init parser
    {
        llhttp_settings_init(&iHttpParserSettings);

        iHttpParserSettings.on_url              = SHTTP_OnURL;
        iHttpParserSettings.on_header_field     = SHTTP_OnHeaderField;
        iHttpParserSettings.on_header_value     = SHTTP_OnHeaderValue;
        iHttpParserSettings.on_headers_complete = SHTTP_OnHeadersComplete;
        iHttpParserSettings.on_body             = SHTTP_OnBody;

        iHttpParserSettings.on_message_begin    = SHTTP_OnMessageBegin;
        iHttpParserSettings.on_message_complete = SHTTP_OnMessageComplete;

        llhttp_init(&iHttpParser, HTTP_REQUEST, &iHttpParserSettings);
        iHttpParserSettings.iTask = this;
    }

    //Init RQ
    //iRqData.clear();
    //iRqData.reserve(4096);//TODO: move to config
    //iRqDataLastBytesRead = 0_byte;
    iRqTmpBuffer.reserve(8192);//TODO: move to config
    iRq.Clear();

    //Init RS
    iRsHeadersData.clear();
    iRs.Clear();
    iRsWriteState = RsWriteStateT::WRITE_HEADERS;
    iRsReaderStorage.reset();

    iState = StateT::PARSE_RQ;
}

void    GpHttpServerBasicSocketTask::ClearRqRsCycle (void)
{
    if (iState == StateT::WAIT_FOR_RQ)
    {
        return;
    }

    //Clear parser
    {
        llhttp_finish(&iHttpParser);
    }

    //Clear RQ
    //iRqData.clear();
    //iRqDataLastBytesRead = 0_byte;
    iRqTmpBuffer.clear();
    iRq.Clear();

    //Clear RS
    iRsHeadersData.clear();
    iRs.Clear();
    iRsWriteState = RsWriteStateT::WRITE_HEADERS;
    iRsReaderStorage.reset();

    iState = StateT::WAIT_FOR_RQ;
}

void    GpHttpServerBasicSocketTask::ParseHttp (GpSocket& aSocket)
{
    //Read from socket
    size_t readFromSocket = 0;
    {
        iRqTmpBuffer.clear();
        GpByteWriterStorageByteArray    writerStorage(iRqTmpBuffer);
        GpByteWriter                    writer(writerStorage);

        readFromSocket = aSocket.Read(writer);

        writer.ShrinkToFit();
    }

    if (readFromSocket == 0)
    {
        return;
    }

    const enum llhttp_errno httpParseRes = llhttp_execute
    (
        &iHttpParser,
        reinterpret_cast<const char*>(iRqTmpBuffer.data()),
        iRqTmpBuffer.size()
    );

    THROW_COND_HTTP
    (
        httpParseRes == HPE_OK,
        GpHttpResponseCode::BAD_REQUEST_400,
        [&](){return std::u8string(GpUTF::S_STR_To_UTF8(llhttp_errno_name(httpParseRes)));}
    );
}

int GpHttpServerBasicSocketTask::SHTTP_OnURL
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnURL(GpUTF::S_STR_To_UTF8(aData, aLength));
}

int GpHttpServerBasicSocketTask::HTTP_OnURL (std::u8string_view aValue)
{
    iRq.Vn().url = aValue;
    return HPE_OK;
}

int GpHttpServerBasicSocketTask::SHTTP_OnHeaderField
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnHeaderField(GpUTF::S_STR_To_UTF8(aData, aLength));
}

int GpHttpServerBasicSocketTask::HTTP_OnHeaderField (std::u8string_view aValue)
{

    iHttpParserCurrentHeaderName = aValue;
    return HPE_OK;
}

int GpHttpServerBasicSocketTask::SHTTP_OnHeaderValue
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnHeaderValue(GpUTF::S_STR_To_UTF8(aData, aLength));
}

int GpHttpServerBasicSocketTask::HTTP_OnHeaderValue (std::u8string_view aValue)
{
    iRq.Vn().headers.GpHttpProtoHeaders::Add(std::move(iHttpParserCurrentHeaderName), aValue);

    return HPE_OK;
}

int GpHttpServerBasicSocketTask::SHTTP_OnHeadersComplete (llhttp_t* aHttp)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnHeadersComplete(aHttp);
}

int GpHttpServerBasicSocketTask::HTTP_OnHeadersComplete (llhttp_t* /*aHttp*/)
{
    //TODO: add header check
    bool hasContentLengthHeader = true;

    //Check if there no body
    if (!hasContentLengthHeader)
    {
        return 1;
    }  else
    {
        return HPE_OK;
    }
}

int GpHttpServerBasicSocketTask::SHTTP_OnBody
(
    llhttp_t*       aHttp,
    const char*     aData,
    const size_t    aLength
)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnBody(GpUTF::S_STR_To_UTF8(aData, aLength));
}

int GpHttpServerBasicSocketTask::HTTP_OnBody (std::u8string_view aValue)
{
    GpBytesArray& rqBody = iRq.Vn().body;

    rqBody.reserve(std::max(rqBody.size(), size_t(512)));

    GpByteWriterStorageByteArray    writerStorage(rqBody);
    GpByteWriter                    writer(writerStorage);

    writer.Offset(rqBody.size());
    writer.Bytes(aValue);

    return HPE_OK;
}

int GpHttpServerBasicSocketTask::SHTTP_OnMessageBegin (llhttp_t* aHttp)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnMessageBegin(aHttp);
}

int GpHttpServerBasicSocketTask::HTTP_OnMessageBegin (llhttp_t* /*aHttp*/)
{
    iRq = MakeSP<GpHttpRequest>();
    return HPE_OK;
}

int GpHttpServerBasicSocketTask::SHTTP_OnMessageComplete (llhttp_t* aHttp)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnMessageComplete(aHttp);
}

int GpHttpServerBasicSocketTask::HTTP_OnMessageComplete (llhttp_t* aHttp)
{
    //Version
    {
        iRq.Vn().SetHttpVersion(aHttp->http_major, aHttp->http_minor);
    }

    //Request type
    {
        GpHttpRequestType::EnumT httpRequestType;
        switch (aHttp->method)
        {
            case HTTP_GET:      httpRequestType = GpHttpRequestType::GET; break;
            case HTTP_HEAD:     httpRequestType = GpHttpRequestType::HEAD; break;
            case HTTP_POST:     httpRequestType = GpHttpRequestType::POST; break;
            case HTTP_PUT:      httpRequestType = GpHttpRequestType::PUT; break;
            case HTTP_DELETE:   httpRequestType = GpHttpRequestType::DELETE; break;
            case HTTP_CONNECT:  httpRequestType = GpHttpRequestType::CONNECT; break;
            case HTTP_OPTIONS:  httpRequestType = GpHttpRequestType::OPTIONS; break;
            case HTTP_TRACE:    httpRequestType = GpHttpRequestType::TRACE; break;
            case HTTP_PATCH:    httpRequestType = GpHttpRequestType::PATCH; break;
            default:
            {
                THROW_HTTP(GpHttpResponseCode::BAD_REQUEST_400, "Unsupported HTTP method"_sv);
            }
        }

        iRq.Vn().request_type = httpRequestType;
    }

    iState  = StateT::PROCESS_RQ_READY_TO;
    return HPE_OK;
}

}//namespace GPlatform

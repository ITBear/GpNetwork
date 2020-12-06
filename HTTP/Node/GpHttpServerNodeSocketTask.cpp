#include "GpHttpServerNodeSocketTask.hpp"
#include "../GpHttpRequestTask.hpp"
#include "../GpHttpResponseSerializer.hpp"
#include <iostream>

namespace GPlatform {

//https://llhttp.org/
//https://pypi.org/project/llhttp/

GpHttpServerNodeSocketTask::GpHttpServerNodeSocketTask (GpIOEventPoller::WP             aIOPooler,
                                                        GpSocket::SP                    aSocket,
                                                        GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
GpSocketTask(std::move(aIOPooler), std::move(aSocket)),
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

GpHttpServerNodeSocketTask::~GpHttpServerNodeSocketTask (void) noexcept
{
}

GpTask::ResT    GpHttpServerNodeSocketTask::OnStep (EventOptRefT aEvent)
{
    if (aEvent.has_value())
    {
        const GpEvent&  event           = aEvent.value().get();
        const GpUUID    eventTypeUID    = event.TypeInfo().UID();
        if (eventTypeUID == GpIOEvent::STypeUID())
        {
            return GpSocketTask::OnStep(aEvent);
        } else if (eventTypeUID == GpHttpRequestDoneEvent::STypeUID())
        {
            return ProcessDoneEvent(static_cast<const GpHttpRequestDoneEvent&>(event));
        }
    }

    return GpTask::ResT::WAITING;
}

GpTask::ResT    GpHttpServerNodeSocketTask::OnSockReadyToRead (GpSocket& aSocket)
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
            GpHttpRequestTask::SP   requestTask     = MakeSP<GpHttpRequestTask>(iRq,
                                                                                iRequestHandlerFactory->NewInstance(),
                                                                                GetWeakPtr());
            GpTaskScheduler::WP     taskScheduler   = GpTaskScheduler::SCurrentScheduler().value();

            taskScheduler->AddTaskToReady(std::move(requestTask));
            iState = StateT::PROCESS_RQ_IN_PROGRESS;
            return GpTask::ResT::WAITING;
        }

        if (   (iState == StateT::PROCESS_RQ_DONE_WRITE_RS)
            || (iState == StateT::PROCESS_RQ_IN_PROGRESS))

        {
            //Data income while write RS
            THROW_GPE("Data income while RS in progress"_sv);
        }
    } catch (const GpHttpException& httpEx)
    {
        iRs     = MakeSP<GpHttpResponse>();
        iRs->SetFromException(httpEx);
        iState  = StateT::PROCESS_RQ_DONE_WRITE_RS;
        iRs.Vn().http_version = iRq.Vn().http_version;
        return WriteToSocket(aSocket);
    }

    return GpTask::ResT::WAITING;
}

GpTask::ResT    GpHttpServerNodeSocketTask::OnSockReadyToWrite (GpSocket& aSocket)
{
    return WriteToSocket(aSocket);
}

void    GpHttpServerNodeSocketTask::OnSockClosed (GpSocket& /*aSocket*/)
{
    ClearRqRsCycle();
}

void    GpHttpServerNodeSocketTask::OnSockError (GpSocket& /*aSocket*/)
{
    ClearRqRsCycle();
}

GpTask::ResT    GpHttpServerNodeSocketTask::ProcessDoneEvent (const GpHttpRequestDoneEvent& aRequestDoneEvent)
{
    THROW_GPE_COND_CHECK_M(iState == StateT::PROCESS_RQ_IN_PROGRESS,
                           "State expected to be 'PROCESS_RQ_IN_PROGRESS'"_sv);

    iRs     = aRequestDoneEvent.Response();
    iState  = StateT::PROCESS_RQ_DONE_WRITE_RS;
    iRs.Vn().http_version = iRq.Vn().http_version;
    return WriteToSocket(Socket());
}

GpTask::ResT    GpHttpServerNodeSocketTask::WriteToSocket (GpSocket& aSocket)
{
    if (iState != StateT::PROCESS_RQ_DONE_WRITE_RS)
    {
        return GpTask::ResT::WAITING;
    }

    //Serialize rs headers
    if (iRsHeadersData.size() == 0)
    {
        const GpBytesArray& body = iRs->body;
        iRs->headers.SetContentLength(body.size());

        iRsHeadersData.reserve(512);//TODO: move to config
        GpByteWriterStorageByteArray    rsHeadersDataStorage(iRsHeadersData);
        GpByteWriter                    rsHeadersWriter(rsHeadersDataStorage);
        GpHttpResponseSerializer::SSerializeHeaders(iRs.VC(), rsHeadersWriter);

        iRsReaderStorage = GpByteReaderStorage(iRsHeadersData);
    }

    //Write headers/body
    if (iRsReaderStorage.has_value())
    {
        GpByteReaderStorage& rsDataReaderStorage = iRsReaderStorage.value();

        if (rsDataReaderStorage.SizeLeft() > 0_byte)
        {
            GpByteReader        reader(rsDataReaderStorage);
            const size_byte_t   bytesWrite  = aSocket.Write(reader);
            if (bytesWrite == 0_byte)//No space left in socket buffer
            {
                return GpTask::ResT::WAITING;
            } else if (rsDataReaderStorage.SizeLeft() > 0_byte)//No space left in socket buffer
            {
                return GpTask::ResT::WAITING;
            } else//All data has been write to socket
            {
                if (iRsWriteState == RsWriteStateT::WRITE_HEADERS)
                {
                    //Check if there are body
                    const GpBytesArray& body = iRs->body;
                    if (body.size() > 0)
                    {
                        iRsWriteState       = RsWriteStateT::WRITE_BODY;
                        iRsReaderStorage    = GpByteReaderStorage(body);

                        return WriteToSocket(aSocket);
                    }
                } else if (iRsWriteState == RsWriteStateT::WRITE_BODY)
                {
                    //NOP, go to finish
                }
            }
        }
    }

    //Finish
    //if (iRs.VCn().connection_flag == GpHttpConnectionFlag::KEEP_ALIVE)
    //{
    ClearRqRsCycle();
    return  GpTask::ResT::WAITING;//TODO: add timeout
    //} else
    //{
    //  ClearRqRsCycle();
    //  return GpTask::ResT::DONE;
    //}
}

void    GpHttpServerNodeSocketTask::InitRqRsCycle (void)
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
    iRqData.clear();
    iRqData.reserve(4096);//TODO: move to config
    iRqDataLastBytesRead = 0_byte;
    iRq.Clear();

    //Init RS
    iRsHeadersData.clear();
    iRs.Clear();
    iRsWriteState = RsWriteStateT::WRITE_HEADERS;
    iRsReaderStorage.reset();

    iState = StateT::PARSE_RQ;
}

void    GpHttpServerNodeSocketTask::ClearRqRsCycle (void)
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
    iRqData.clear();
    iRqDataLastBytesRead = 0_byte;
    iRq.Clear();

    //Clear RS
    iRsHeadersData.clear();
    iRs.Clear();
    iRsWriteState = RsWriteStateT::WRITE_HEADERS;
    iRsReaderStorage.reset();

    iState = StateT::WAIT_FOR_RQ;
}

void    GpHttpServerNodeSocketTask::ParseHttp (GpSocket& aSocket)
{
    iRqDataLastBytesRead = 0_byte;

    GpByteWriterStorageByteArray    writerStorage(iRqData);
    GpByteWriter                    writer(writerStorage);

    iRqDataLastBytesRead = aSocket.Read(writer);

    if (iRqDataLastBytesRead == 0_byte)
    {
        return;
    }

    GpRawPtrCharR requestData(iRqData);
    GpRawPtrCharR requestNewPartData = requestData.SubrangeEndOffset(iRqDataLastBytesRead.As<count_t>());

    const enum llhttp_errno httpParseRes = llhttp_execute(&iHttpParser,
                                                          requestNewPartData.PtrBegin(),
                                                          requestNewPartData.CountTotal().As<int>());

    THROW_HTTP_COND_CHECK_M(httpParseRes == HPE_OK,
                            GpHttpResponseCode::BAD_REQUEST_400,
                            llhttp_errno_name(httpParseRes));
}

int GpHttpServerNodeSocketTask::SHTTP_OnURL (llhttp_t* aHttp, const char* aData, const size_t aLength)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnURL(std::string_view(aData, aLength));
}

int GpHttpServerNodeSocketTask::HTTP_OnURL (std::string_view aValue)
{
    iRq.Vn().url = aValue;
    return HPE_OK;
}

int GpHttpServerNodeSocketTask::SHTTP_OnHeaderField (llhttp_t* aHttp, const char* aData, const size_t aLength)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnHeaderField(std::string_view(aData, aLength));
}

int GpHttpServerNodeSocketTask::HTTP_OnHeaderField (std::string_view aValue)
{

    iHttpParserCurrentHeaderName = aValue;
    return HPE_OK;
}

int GpHttpServerNodeSocketTask::SHTTP_OnHeaderValue (llhttp_t* aHttp, const char* aData, const size_t aLength)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnHeaderValue(std::string_view(aData, aLength));
}

int GpHttpServerNodeSocketTask::HTTP_OnHeaderValue (std::string_view aValue)
{
    iRq.Vn().headers.Add(std::move(iHttpParserCurrentHeaderName), aValue);

    return HPE_OK;
}

int GpHttpServerNodeSocketTask::SHTTP_OnHeadersComplete (llhttp_t* aHttp)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnHeadersComplete(aHttp);
}

int GpHttpServerNodeSocketTask::HTTP_OnHeadersComplete (llhttp_t* /*aHttp*/)
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

int GpHttpServerNodeSocketTask::SHTTP_OnBody (llhttp_t* aHttp, const char* aData, const size_t aLength)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnBody(std::string_view(aData, aLength));
}

int GpHttpServerNodeSocketTask::HTTP_OnBody (std::string_view aValue)
{
    iRq.Vn().body = GpBytesArrayUtils::SMake(aValue);
    return HPE_OK;
}

int GpHttpServerNodeSocketTask::SHTTP_OnMessageBegin (llhttp_t* aHttp)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnMessageBegin(aHttp);
}

int GpHttpServerNodeSocketTask::HTTP_OnMessageBegin (llhttp_t* /*aHttp*/)
{
    //std::cout << "[GpHttpServerNodeSocketTask::HTTP_OnMessageBegin]: Message begin..."_sv << std::endl;
    iRq = MakeSP<GpHttpRequest>();
    return HPE_OK;
}

int GpHttpServerNodeSocketTask::SHTTP_OnMessageComplete (llhttp_t* aHttp)
{
    return SHTTPSettings(aHttp).iTask->HTTP_OnMessageComplete(aHttp);
}

int GpHttpServerNodeSocketTask::HTTP_OnMessageComplete (llhttp_t* aHttp)
{
    //std::cout << "[GpHttpServerNodeSocketTask::HTTP_OnMessageComplete]: Message complete..."_sv << std::endl;

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

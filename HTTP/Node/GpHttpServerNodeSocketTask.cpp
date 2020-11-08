#include "GpHttpServerNodeSocketTask.hpp"
#include "../GpHttpResponseSerializer.hpp"
#include <iostream>

namespace GPlatform {

//https://llhttp.org/
//https://pypi.org/project/llhttp/

GpHttpServerNodeSocketTask::GpHttpServerNodeSocketTask (GpIOEventPoller::WP aIOPooler,
                                                        GpSocket::SP        aSocket) noexcept:
GpSocketTask(std::move(aIOPooler), std::move(aSocket))
{
}

GpHttpServerNodeSocketTask::~GpHttpServerNodeSocketTask (void) noexcept
{
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

        if (iState == StateT::PROCESS_RQ)
        {
            //TODO: Do something with iRequest
            //iRq.

            iRs     = MakeSP<GpHttpResponse>();
            iState  = StateT::WRITE_RS;
            return WriteToSocket(aSocket);
        }

        if (iState == StateT::WRITE_RS)
        {
            //Data income while write RS
            THROW_GPE("Data income while write RS"_sv);
        }
    } catch (const GpHttpException& httpEx)
    {
        iRs     = MakeSP<GpHttpResponse>(httpEx);
        iState  = StateT::WRITE_RS;
        return WriteToSocket(aSocket);
    }

    return GpTask::ResT::WAITING;
}

GpTask::ResT    GpHttpServerNodeSocketTask::OnSockReadyToWrite (GpSocket& aSocket)
{
    iIsReadyToWriteSocket = true;
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

GpTask::ResT    GpHttpServerNodeSocketTask::WriteToSocket (GpSocket& aSocket)
{
    if (iIsReadyToWriteSocket == false)
    {
        return GpTask::ResT::WAITING;
    }

    if (iState != StateT::WRITE_RS)
    {
        return GpTask::ResT::WAITING;
    }

    //Serialize rs headers
    if (iRsHeadersData.size() == 0)
    {
        iRsHeadersData.reserve(512);//TODO: move to config
        GpByteWriterStorageByteArray    rsHeadersDataStorage(iRsHeadersData);
        GpByteWriter                    rsHeadersWriter(rsHeadersDataStorage);
        GpHttpResponseSerializer::SSerializeHeaders(iRs.VC(), rsHeadersWriter);

        iRsDataReaderStorage = GpByteReaderStorage(iRsHeadersData);
    }

    //Write headers/body
    if (iRsDataReaderStorage.has_value())
    {
        GpByteReaderStorage& rsDataReaderStorage = iRsDataReaderStorage.value();

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
                    const GpBytesArray& body = iRs->Body();
                    if (body.size() > 0)
                    {
                        iRsWriteState           = RsWriteStateT::WRITE_BODY;
                        iRsDataReaderStorage    = GpByteReaderStorage(body);

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
    if (iRs->ConnectionFlag() == GpHttpConnectionFlag::KEEP_ALIVE)
    {
        ClearRqRsCycle();
        return  GpTask::ResT::WAITING;
    } else
    {
        return GpTask::ResT::DONE;
    }
}

void    GpHttpServerNodeSocketTask::InitRqRsCycle (void)
{
    //Init parser
    {
        llhttp_settings_init(&iHttpParserSettings);

        iHttpParserSettings.on_message_begin    = SHTTP_HandleOnMessageBegin;
        iHttpParserSettings.on_message_complete = SHTTP_HandleOnMessageComplete;

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
    iRsDataReaderStorage.reset();

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
    iRsDataReaderStorage.reset();

    iState = StateT::WAIT_FOR_RQ;
}

void    GpHttpServerNodeSocketTask::ParseHttp (GpSocket& aSocket)
{
    iRqDataLastBytesRead = 0_byte;

    GpByteWriterStorageByteArray    writerStorage(iRqData);
    GpByteWriter                    writer(writerStorage);

    iRqDataLastBytesRead = aSocket.Read(writer);

    GpRawPtrCharR requestData(iRqData);
    GpRawPtrCharR requestNewPartData = requestData.SubrangeEndOffset(iRqDataLastBytesRead.As<count_t>());

    const enum llhttp_errno httpParseRes = llhttp_execute(&iHttpParser,
                                                          requestNewPartData.PtrBegin(),
                                                          requestNewPartData.CountTotal().As<int>());

    THROW_HTTP_COND_CHECK_M(httpParseRes == HPE_OK,
                            GpHttpResponseCode::BAD_REQUEST_400,
                            llhttp_errno_name(httpParseRes));
}

int GpHttpServerNodeSocketTask::SHTTP_HandleOnMessageBegin (llhttp_t* aHttp)
{
    const http_settings_t& settings = *static_cast<const http_settings_t*>(aHttp->settings);
    return settings.iTask->HTTP_HandleOnMessageBegin(aHttp);
}

int GpHttpServerNodeSocketTask::HTTP_HandleOnMessageBegin (llhttp_t* /*aHttp*/)
{
    std::cout << "[GpHttpServerNodeSocketTask::HTTP_HandleOnMessageBegin]: Message begin..."_sv << std::endl;
    return HPE_OK;
}

int GpHttpServerNodeSocketTask::SHTTP_HandleOnMessageComplete (llhttp_t* aHttp)
{
    const http_settings_t& settings = *static_cast<const http_settings_t*>(aHttp->settings);
    return settings.iTask->HTTP_HandleOnMessageComplete(aHttp);
}

int GpHttpServerNodeSocketTask::HTTP_HandleOnMessageComplete (llhttp_t* /*aHttp*/)
{
    std::cout << "[GpHttpServerNodeSocketTask::HTTP_HandleOnMessageComplete]: Message complete..."_sv << std::endl;

    iRq     = MakeSP<GpHttpRequest>();
    iState  = StateT::PROCESS_RQ;
    return HPE_OK;
}

}//namespace GPlatform

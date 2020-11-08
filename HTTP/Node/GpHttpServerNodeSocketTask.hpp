#pragma once

#include "../../IO/Sockets/GpSocketTask.hpp"
#include "../GpHttpRequestSerializer.hpp"
#include "../GpHttpResponseSerializer.hpp"
#include "../GpHttpException.hpp"

#include <llhttp/llhttp.h>

namespace GPlatform {

class GpHttpServerNodeSocketTask final: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS(GpHttpServerNodeSocketTask)
    CLASS_DECLARE_DEFAULTS(GpHttpServerNodeSocketTask)

    struct http_settings_t: public llhttp_settings_t
    {
        GpHttpServerNodeSocketTask* iTask = nullptr;
    };

    enum class StateT
    {
        WAIT_FOR_RQ,
        PARSE_RQ,
        PROCESS_RQ,
        WRITE_RS
    };

    enum class RsWriteStateT
    {
        WRITE_HEADERS,
        WRITE_BODY
    };

public:
                            GpHttpServerNodeSocketTask      (GpIOEventPoller::WP    aIOPooler,
                                                             GpSocket::SP           aSocket) noexcept;
    virtual                 ~GpHttpServerNodeSocketTask     (void) noexcept override final;

protected:
    virtual GpTask::ResT    OnSockReadyToRead               (GpSocket& aSocket) override final;
    virtual GpTask::ResT    OnSockReadyToWrite              (GpSocket& aSocket) override final;
    virtual void            OnSockClosed                    (GpSocket& aSocket) override final;
    virtual void            OnSockError                     (GpSocket& aSocket) override final;

private:
    GpTask::ResT            WriteToSocket                   (GpSocket& aSocket);

    void                    InitRqRsCycle                   (void);
    void                    ClearRqRsCycle                  (void);
    void                    ParseHttp                       (GpSocket& aSocket);

    static int              SHTTP_HandleOnMessageBegin      (llhttp_t* aHttp);
    int                     HTTP_HandleOnMessageBegin       (llhttp_t* aHttp);

    static int              SHTTP_HandleOnMessageComplete   (llhttp_t* aHttp);
    int                     HTTP_HandleOnMessageComplete    (llhttp_t* aHttp);

private:
    StateT                  iState                  = StateT::WAIT_FOR_RQ;

    //Rq parser
    llhttp_t                iHttpParser;
    http_settings_t         iHttpParserSettings;

    //Rq
    GpBytesArray            iRqData;
    size_byte_t             iRqDataLastBytesRead    = 0_byte;
    GpHttpRequest::SP       iRq;

    //Rs
    bool                    iIsReadyToWriteSocket   = false;
    GpBytesArray            iRsHeadersData;
    GpHttpResponse::SP      iRs;
    RsWriteStateT           iRsWriteState           = RsWriteStateT::WRITE_HEADERS;
    std::optional<GpByteReaderStorage>  iRsDataReaderStorage;
};

}//namespace GPlatform

#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTask.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Routers/GpHttpRouter.hpp>
#include <GpNetwork/GpNetworkHttp/llhttp/llhttp.h>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestSocketTask final: public GpSingleSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpRequestSocketTask)
    CLASS_DD(GpHttpRequestSocketTask)

    enum class ProcessStateT
    {
        WAIT_FOR_RQ,
        PARSE_RQ,
        RQ_READY,
        RS_IN_PROGRESS,
        WRITE_RS
    };

    enum class RsWriteStateT
    {
        WRITE_HEADERS_S0,   //
        WRITE_HEADERS_S1,   //
        WRITE_BODY_S0,      //
        WRITE_BODY_S1,      //
        WRITE_DONE_S0,      //
        WRITE_DONE_S1       //
    };

    struct http_settings_t: public llhttp_settings_t
    {
        GpHttpRequestSocketTask* iHttpRequestSocketTask = nullptr;
    };

public:
                            GpHttpRequestSocketTask     (GpSocket::SP       aSocket,
                                                         GpHttpRouter::SP   aRouter,
                                                         GpIOEventPollerIdx aIOEventPollerIdx) noexcept;
    virtual                 ~GpHttpRequestSocketTask    (void) noexcept override final;

    virtual void            OnReadyToRead               (GpSocket& aSocket) override final;
    virtual void            OnReadyToWrite              (GpSocket& aSocket) override final;
    virtual void            OnClosed                    (GpSocket& aSocket) override final;
    virtual void            OnError                     (GpSocket& aSocket) override final;
    virtual void            ProcessOtherMessages        (GpAny& aMessage) override final;

    void                    ReadNextAndParseRq          (GpSocket& aSocket);

    static int              SHttp_OnURL                 (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                     Http_OnURL                  (std::string_view aValue);

    static int              SHttp_OnHeaderField         (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                     Http_OnHeaderField          (std::string_view aValue);

    static int              SHttp_OnHeaderValue         (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                     Http_OnHeaderValue          (std::string_view aValue);

    static int              SHttp_OnHeadersComplete     (llhttp_t* aHttp);
    int                     Http_OnHeadersComplete      (llhttp_t* aHttp);

    static int              SHttp_OnBody                (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                     Http_OnBody                 (GpSpanByteR aValue);

    static int              SHttp_OnMessageBegin        (llhttp_t* aHttp);
    int                     Http_OnMessageBegin         (llhttp_t* aHttp);

    static int              SHttp_OnMessageComplete     (llhttp_t* aHttp);
    int                     Http_OnMessageComplete      (llhttp_t* aHttp);

    inline static const     http_settings_t&
                            SHttpSettings               (llhttp_t* aHttp);

private:
    void                    InitCycle                   (void);
    void                    FinishCycle                 (void);

    static size_t           SReadFromSocket             (GpSocket&      aSocket,
                                                         GpBytesArray&  aBufferOut);
    void                    WriteRsToSocket             (GpSocket&      aSocket);

private:
    // Http router
    GpHttpRouter::SP        iRouter;

    // RQ
    ProcessStateT           iProcessState           = ProcessStateT::WAIT_FOR_RQ;
    llhttp_t                iHttpParser;
    http_settings_t         iHttpParserSettings;
    std::string             iHttpParserCurrentHeaderName;
    GpHttpRequestNoBodyDesc iRequestNoBody;
    GpHttpBodyPayload::SP   iRequestBodyPayloadSP;
    GpBytesArray            iSocketTmpBuffer;

    // RS
    RsWriteStateT           iRsWriteState           = RsWriteStateT::WRITE_HEADERS_S0;
    GpHttpResponse::SP      iHttpResponseSP;
    size_t                  iRsBytesToWriteTotal    = 0;
    size_t                  iRsBytesWrited          = 0;
};

const GpHttpRequestSocketTask::http_settings_t& GpHttpRequestSocketTask::SHttpSettings (llhttp_t* aHttp)
{
    return *static_cast<const http_settings_t*>(aHttp->settings);
}

}// namespace GPlatform

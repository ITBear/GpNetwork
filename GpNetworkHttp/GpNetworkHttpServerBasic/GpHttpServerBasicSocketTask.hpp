#pragma once

#include "GpNetworkHttpServerBasic_global.hpp"

#if  __has_include(<llhttp/llhttp.h>)
#   include <llhttp/llhttp.h>
#else
#   include "../llhttp/llhttp.h"
#endif

namespace GPlatform {

class GpHttpServerBasicSocketTask final: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerBasicSocketTask)
    CLASS_DD(GpHttpServerBasicSocketTask)

    struct http_settings_t: public llhttp_settings_t
    {
        GpHttpServerBasicSocketTask*    iTask = nullptr;
    };

    enum class StateT
    {
        WAIT_FOR_RQ,
        PARSE_RQ,
        PROCESS_RQ_READY_TO,
        PROCESS_RQ_IN_PROGRESS,
        PROCESS_RQ_DONE_WRITE_RS
    };

    enum class RsWriteStateT
    {
        WRITE_HEADERS,
        WRITE_BODY
    };

public:
    inline                          GpHttpServerBasicSocketTask (std::string                        aName,
                                                                 GpIOEventPoller::SP                aIOPoller,
                                                                 GpSocket::SP                       aSocket,
                                                                 GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;
    virtual                         ~GpHttpServerBasicSocketTask(void) noexcept override final;

protected:
    virtual GpTaskDoRes             OnStep                      (EventOptRefT aEvent) override final;

    virtual GpTaskDoRes             OnSockReadyToRead           (GpSocket& aSocket) override final;
    virtual GpTaskDoRes             OnSockReadyToWrite          (GpSocket& aSocket) override final;
    virtual void                    OnSockClosed                (GpSocket& aSocket) override final;
    virtual void                    OnSockError                 (GpSocket& aSocket) override final;

private:
    GpTaskDoRes                     ProcessDoneEvent            (const GpHttpRequestDoneEvent& aRequestDoneEvent);

    GpTaskDoRes                     WriteToSocket               (GpSocket& aSocket);

    void                            InitRqRsCycle               (void);
    void                            ClearRqRsCycle              (void);
    void                            ParseHttp                   (GpSocket& aSocket);

    static int                      SHTTP_OnURL                 (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                             HTTP_OnURL                  (std::string_view aValue);

    static int                      SHTTP_OnHeaderField         (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                             HTTP_OnHeaderField          (std::string_view aValue);

    static int                      SHTTP_OnHeaderValue         (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                             HTTP_OnHeaderValue          (std::string_view aValue);

    static int                      SHTTP_OnHeadersComplete     (llhttp_t* aHttp);
    int                             HTTP_OnHeadersComplete      (llhttp_t* aHttp);

    static int                      SHTTP_OnBody                (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                             HTTP_OnBody                 (std::string_view aValue);

    static int                      SHTTP_OnMessageBegin        (llhttp_t* aHttp);
    int                             HTTP_OnMessageBegin         (llhttp_t* aHttp);

    static int                      SHTTP_OnMessageComplete     (llhttp_t* aHttp);
    int                             HTTP_OnMessageComplete      (llhttp_t* aHttp);

    static const http_settings_t&   SHTTPSettings               (llhttp_t* aHttp)
    {
        return *static_cast<const http_settings_t*>(aHttp->settings);
    }

private:
    GpHttpRequestHandlerFactory::SP     iRequestHandlerFactory;
    StateT                              iState                  = StateT::WAIT_FOR_RQ;

    //Rq parser
    llhttp_t                            iHttpParser;
    http_settings_t                     iHttpParserSettings;
    std::string                         iHttpParserCurrentHeaderName;

    //Rq
    //GpBytesArray                      iRqData;
    //size_byte_t                       iRqDataLastBytesRead    = 0_byte;
    GpBytesArray                        iRqTmpBuffer;
    GpHttpRequest::SP                   iRq;

    //Rs
    GpBytesArray                        iRsHeadersData;
    GpHttpResponse::SP                  iRs;
    RsWriteStateT                       iRsWriteState           = RsWriteStateT::WRITE_HEADERS;
    std::optional<GpByteReaderStorage>  iRsReaderStorage;
};

GpHttpServerBasicSocketTask::GpHttpServerBasicSocketTask
(
    std::string                     aName,
    GpIOEventPoller::SP             aIOPoller,
    GpSocket::SP                    aSocket,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
) noexcept:
GpSocketTask
(
    std::move(aName),
    std::move(aIOPoller),
    std::move(aSocket)
),
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

}//namespace GPlatform

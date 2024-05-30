#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpClientCurl/GpNetworkHttpClientCurl_global.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Client/GpHttpClient.hpp>

typedef void CURL;
typedef void CURLM;

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CLIENT_CURL_API, GpHttpClientCurlAsyncState,
    READY_FOR_REQUEST,
    CHECK_CONNECTION,
    CONNECTION_IN_PROGRESS,
    CONNECTED_WAIT_FOR_REQUEST,
    REQUEST_IN_PROGRESS,
    REQUEST_DONE_WAIT_FOR_RESPONSE,
    RESPONSE_IN_PROGRESS
);

class GP_NETWORK_HTTP_CLIENT_CURL_API GpHttpClientCurlAsync final: public GpHttpClient
{   
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClientCurlAsync)
    CLASS_DD(GpHttpClientCurlAsync)

    using StateT    = GpHttpClientCurlAsyncState;
    using StateTE   = StateT::EnumT;

public:
                                GpHttpClientCurlAsync   (GpSocketFlags      aSocketFlags,
                                                         GpIOEventPollerIdx aIOEventPollerIdx,
                                                         milliseconds_t     aConnectTimeout);
                                GpHttpClientCurlAsync   (GpSocketFlags      aSocketFlags,
                                                         GpIOEventPollerIdx aIOEventPollerIdx,
                                                         milliseconds_t     aConnectTimeout,
                                                         std::string        aTaskName);
    virtual                     ~GpHttpClientCurlAsync  (void) noexcept override final;

    virtual GpHttpResponse::SP  DoRqAndWaitForRs        (GpHttpRequest::SP  aRequestSP,
                                                         ErorrMode          aErorrMode) override final;

protected:
    virtual void                OnReadyToRead           (GpSocket& aSocket) override final;
    virtual void                OnReadyToWrite          (GpSocket& aSocket) override final;
    virtual void                OnClosed                (GpSocket& aSocket) override final;
    virtual void                OnError                 (GpSocket& aSocket) override final;
    virtual void                ProcessOtherMessages    (GpAny& aMessage) override final;

private:
    void                        CheckConnection         (const GpHttpRequest& aRequest);
    GpHttpResponse::SP          DoCurlRequestAndWait    (GpHttpRequest& aRequest,
                                                         ErorrMode      aErorrMode);

    void                        CurlInit                (void);
    void                        CurlClear               (void) noexcept;
    GpSocketId                  SocketId                (void) const;
    GpSocketId                  SocketIdNoEx            (void) const noexcept;

private:
    mutable GpSpinLock          iSpinLock;
    StateTE                     iState          GUARDED_BY(iSpinLock) = StateT::READY_FOR_REQUEST;
    GpSocketAddr                iConnecedToAddr GUARDED_BY(iSpinLock);

    CURL*                       iCurl           = nullptr;
    CURLM*                      iCurlMulti      = nullptr;
};

}// namespace GPlatform

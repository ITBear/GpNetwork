#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpTcpClientTask.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpRequest.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpResponse.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpParser.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpClientRequestTask final: public GpTcpClientTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClientRequestTask)
    CLASS_DD(GpHttpClientRequestTask)

    enum class ProcessStateT
    {
        CHECK_CONNECTION,
        WRITE_RQ,
        WAIT_FOR_AND_READ_RS
    };

    enum class RqWriteStateT
    {
        WRITE_HEADERS_S0,   //
        WRITE_HEADERS_S1,   //
        WRITE_BODY_S0,      //
        WRITE_BODY_S1,      //
        WRITE_DONE_S0,      //
        WRITE_DONE_S1       //
    };

public:
                    GpHttpClientRequestTask     (GpSocketFlags      aSocketFlags,
                                                 GpIOEventPollerIdx aIOEventPollerIdx,
                                                 GpHttpRequest::UP  aRequestUP,
                                                 milliseconds_t     aConnectTimeout);
                    GpHttpClientRequestTask     (GpSocketFlags      aSocketFlags,
                                                 GpIOEventPollerIdx aIOEventPollerIdx,
                                                 GpHttpRequest::UP  aRequestUP,
                                                 milliseconds_t     aConnectTimeout,
                                                 std::string        aTaskName);
    virtual         ~GpHttpClientRequestTask    (void) noexcept override final;

protected:
    virtual void    OnStart                     (void) override final;
    virtual void    OnStop                      (ExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void    OnStopException             (const GpException& aException) noexcept override final;

    virtual void    OnReadyToRead               (GpSocket& aSocket) REQUIRES(SpinLock()) override final;
    virtual void    OnReadyToWrite              (GpSocket& aSocket) REQUIRES(SpinLock()) override final;
    virtual void    OnClosed                    (GpSocket& aSocket) REQUIRES(SpinLock()) override final;
    virtual void    OnError                     (GpSocket& aSocket) REQUIRES(SpinLock()) override final;

    virtual void    OnConnected                 (GpSocketTCP& aSocket) REQUIRES(SpinLock()) override final;
    virtual void    OnConnectionTimeout         (GpSocketTCP& aSocket) REQUIRES(SpinLock()) override final;

private:
    void            WriteRqToSocket             (GpSocket& aSocket);

private:
    const GpIOEventPollerIdx    iIOEventPollerIdx;
    ProcessStateT               iProcessState           = ProcessStateT::CHECK_CONNECTION;
    GpByteArray                 iSocketTmpBuffer;

    // RQ
    GpHttpRequest::UP           iRqUP;
    RqWriteStateT               iRqWriteState           = RqWriteStateT::WRITE_HEADERS_S0;
    size_t                      iRqBytesToWriteTotal    = 0;
    size_t                      iRqBytesWrited          = 0;
    const milliseconds_t        iConnectTimeout;

    // RS
    GpHttpParser                iHttpParser;
};

}// namespace GPlatform

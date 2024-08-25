#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCore_global.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Routers/GpHttpRouter.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpTcpServerTask.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpParser.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServerRequestTask final: public GpTcpServerTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerRequestTask)
    CLASS_DD(GpHttpServerRequestTask)

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

public:
                        GpHttpServerRequestTask     (GpSocketTCP::SP    aSocketTCP,
                                                     GpIOEventPollerIdx aIOEventPollerIdx,
                                                     GpHttpRouter::SP   aRouterSP) noexcept;
    virtual             ~GpHttpServerRequestTask    (void) noexcept override final;

protected:
    virtual void        OnStart                     (void) override final;
    virtual void        OnStop                      (StopExceptionsT& aStopExceptionsOut) noexcept override final;
    virtual void        OnStopException             (const GpException& aException) noexcept override final;

    virtual void        OnReadyToRead               (GpSocket& aSocket) override final;
    virtual void        OnReadyToWrite              (GpSocket& aSocket) override final;
    virtual void        OnClosed                    (GpSocket& aSocket) override final;
    virtual void        OnError                     (GpSocket& aSocket) override final;
    virtual void        ProcessOtherMessages        (GpAny& aMessage) override final;

private:
    void                InitCycle                   (void);
    void                FinishCycle                 (void);

    static size_t       SReadFromSocket             (GpSocket&      aSocket,
                                                     GpBytesArray&  aBufferOut);
    void                WriteRsToSocket             (GpSocket&      aSocket);

private:
    GpBytesArray        iSocketTmpBuffer;
    ProcessStateT       iProcessState           = ProcessStateT::WAIT_FOR_RQ;

    // Http router
    GpHttpRouter::SP    iRouterSP;

    // RQ
    GpHttpParser        iHttpParser;

    // RS
    RsWriteStateT       iRsWriteState           = RsWriteStateT::WRITE_HEADERS_S0;
    GpHttpResponse::SP  iRsSP;
    size_t              iRsBytesToWriteTotal    = 0;
    size_t              iRsBytesWrited          = 0;
};

}// namespace GPlatform

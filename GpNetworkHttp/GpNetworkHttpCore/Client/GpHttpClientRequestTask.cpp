#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Client/GpHttpClientRequestTask.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadFixed.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpHttpClientRequestTask::GpHttpClientRequestTask
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    GpHttpRequest::SP           aRequestSP,
    const milliseconds_t        aConnectTimeout
):
GpTcpClientTask
{
    aSocketFlags,
    aIOEventPollerIdx
},
iRqSP          {std::move(aRequestSP)},
iConnectTimeout{aConnectTimeout}
{
}

GpHttpClientRequestTask::GpHttpClientRequestTask
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    GpHttpRequest::SP           aRequestSP,
    const milliseconds_t        aConnectTimeout,
    std::string                 aTaskName
):
GpTcpClientTask
{
    aSocketFlags,
    aIOEventPollerIdx,
    std::move(aTaskName)
},
iRqSP          {std::move(aRequestSP)},
iConnectTimeout{aConnectTimeout}
{
}

GpHttpClientRequestTask::~GpHttpClientRequestTask (void) noexcept
{
}

void    GpHttpClientRequestTask::OnStart (void)
{
    GpTcpClientTask::OnStart();

    iProcessState = ProcessStateT::CHECK_CONNECTION;

    // Get server host name to connect
    const GpUrlAuthority& urlAuthority = iRqSP.Vn().iRequestNoBody.url.Authority();

    // TODO: implement IPv6
    // Connect
    ConnectTcpAndWait
    (
        urlAuthority.Host(),
        urlAuthority.Port(),
        GpSocketIPv::IPv4,
        iConnectTimeout
    );

    iSocketTmpBuffer.resize(4096); // TODO: move to config

    iProcessState = ProcessStateT::WRITE_RQ;
    WriteRqToSocket(SocketTCP());
}

void    GpHttpClientRequestTask::OnStop (StopExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        LOG_INFO("[GpHttpClientRequestTask::OnStop]:..."_sv);
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& ex)
    {
        aStopExceptionsOut.emplace_back(GpException{ex.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpHttpClientRequestTask::OnStop]: unknown exception"_sv});
    }

    GpTcpClientTask::OnStop(aStopExceptionsOut);
}

void    GpHttpClientRequestTask::OnStopException (const GpException& aException) noexcept
{
    LOG_EXCEPTION
    (
        "[GpHttpClientRequestTask::OnStopException]",
        aException
    );
}

void    GpHttpClientRequestTask::OnReadyToRead ([[maybe_unused]] GpSocket& aSocket)
{
    if (iProcessState != ProcessStateT::WAIT_FOR_AND_READ_RS)
    {
        THROW_GP("Incorrect HTTP client state detected while incoming data appeared on the socket"_sv);
    }

    // Read from socket
    GpByteWriterStorageByteArray    writerStorage(iSocketTmpBuffer);
    GpByteWriter                    writer(writerStorage);

    GpSocketTCP& socketTcp          = static_cast<GpSocketTCP&>(aSocket);
    const size_t readFromSocketSize = socketTcp.Read(writer);

    if (readFromSocketSize > 0)
    {
        if (iHttpParser.ParseNext(GpSpanByteR{std::data(iSocketTmpBuffer), readFromSocketSize}) == true)
        {
            // Get RS
            GpHttpResponse::SP httpRsSP = MakeSP<GpHttpResponse>
            (
                iHttpParser.RsNoBody(),
                iHttpParser.BodyPayload()
            );

            // Fulfill done future
            DonePromise().Fulfill(DonePromiseRes{std::move(httpRsSP)});

            // TODO:
            // If keep-allive, move socket to pool

            // Done task
            RequestTaskStop();
        }
    }
}

void    GpHttpClientRequestTask::OnReadyToWrite ([[maybe_unused]] GpSocket& aSocket)
{
    if (iProcessState != ProcessStateT::WRITE_RQ)
    {
        return;
    }

    WriteRqToSocket(aSocket);
}

void    GpHttpClientRequestTask::OnClosed ([[maybe_unused]] GpSocket& aSocket)
{
    LOG_INFO("[GpHttpClientRequestTask::OnClosed]"_sv);
}

void    GpHttpClientRequestTask::OnError ([[maybe_unused]] GpSocket& aSocket)
{
    LOG_ERROR("[GpHttpClientRequestTask::OnError]"_sv);
}

void    GpHttpClientRequestTask::OnConnected ([[maybe_unused]] GpSocketTCP& aSocket)
{
    std::string_view serverHost = iRqSP.Vn().iRequestNoBody.url.Authority().Host();

    LOG_INFO
    (
        fmt::format
        (
            "[GpHttpClientRequestTask::OnConnected]: connected to '{}'",
            serverHost
        )
    );
}

void    GpHttpClientRequestTask::ProcessOtherMessages (GpAny& aMessage)
{
#if DEBUG_BUILD
    GpDebugging::SBreakpoint();
#endif// #if DEBUG_BUILD

    LOG_ERROR
    (
        fmt::format
        (
            "[GpHttpClientRequestTask::ProcessOtherMessages]: not socket message {}",
            aMessage.TypeInfo().name()
        )
    );
}

void    GpHttpClientRequestTask::WriteRqToSocket (GpSocket& aSocket)
{
    if (iProcessState != ProcessStateT::WRITE_RQ)
    {
        return;
    }

    // Write headers
    {
        // Serialize headers
        if (iRqWriteState == RqWriteStateT::WRITE_HEADERS_S0)
        {
            // Serialize RQ
            GpByteWriterStorageByteArray        writerStorage(iSocketTmpBuffer);
            GpByteWriter                        writer(writerStorage);
            const GpHttpRequest::SerializeRes   serializeRes = GpHttpRequest::SSerialize(iRqSP.V(), writer);

            iRqBytesToWriteTotal    = writer.TotalWrite();
            iRqBytesWrited          = 0;
            iRqWriteState           = RqWriteStateT::WRITE_HEADERS_S1;

            if (serializeRes == GpHttpRequest::SerializeRes::WRITE_HEADERS_AND_BODY)
            {
                // Write headers and body with single buffer
                iRqSP.Clear();
            }
        }

        // Write headers
        if (iRqWriteState == RqWriteStateT::WRITE_HEADERS_S1)
        {
            GpByteReaderStorage iRqReaderStorage({std::data(iSocketTmpBuffer), iRqBytesToWriteTotal});
            GpByteReader        iRqReader(iRqReaderStorage);
            iRqReader.OffsetAdd(iRqBytesWrited);

            GpSocketTCP& socketTcp  = static_cast<GpSocketTCP&>(aSocket);
            const size_t writeSize  = socketTcp.Write(iRqReader);
            iRqBytesWrited += writeSize;

            if (iRqBytesWrited >= iRqBytesToWriteTotal)
            {
                iRqWriteState = RqWriteStateT::WRITE_BODY_S0;
            }
        }
    }// Write headers

    // Write body
    {
        if (iRqWriteState == RqWriteStateT::WRITE_BODY_S0)
        {
            if (iRqSP.IsNotNULL()) [[likely]]
            {
                GpHttpBodyPayload::SP& bodySP = iRqSP.Vn().iBody;
                if (bodySP.IsNotNULL()) [[likely]]
                {
                    iRqWriteState           = RqWriteStateT::WRITE_BODY_S1;
                    iRqBytesToWriteTotal    = iRqSP.Vn().iBody->Size();
                    iRqBytesWrited          = 0;
                } else
                {
                    iRqWriteState = RqWriteStateT::WRITE_DONE_S0;
                }
            } else
            {
                iRqWriteState = RqWriteStateT::WRITE_DONE_S0;
            }
        }

        if (iRqWriteState == RqWriteStateT::WRITE_BODY_S1)
        {
            // TODO: implement multipart
            const GpSpanByteR rqBodyPayloadFixed = static_cast<const GpHttpBodyPayloadFixed&>(iRqSP.Vn().iBody.V()).Data();

            // -------
            GpByteReaderStorage rqReaderStorage(rqBodyPayloadFixed);
            GpByteReader        rqReader(rqReaderStorage);
            rqReader.OffsetAdd(iRqBytesWrited);

            GpSocketTCP& socketTcp  = static_cast<GpSocketTCP&>(aSocket);
            const size_t writeSize  = socketTcp.Write(rqReader);
            iRqBytesWrited += writeSize;

            if (iRqBytesWrited >= rqBodyPayloadFixed.Count())
            {
                iRqWriteState = RqWriteStateT::WRITE_BODY_S0;
            }
        }
    }// Write body

    // Finish
    if (iRqWriteState == RqWriteStateT::WRITE_DONE_S0)
    {
        iRqWriteState = RqWriteStateT::WRITE_DONE_S1;
        iProcessState = ProcessStateT::WAIT_FOR_AND_READ_RS;

        iHttpParser.InitCycle(GpHttpParser::Mode::HTTP_RESPONSE);
    }
}

}// namespace GPlatform

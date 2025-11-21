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
    GpHttpRequest::UP           aRequestUP,
    const milliseconds_t        aConnectTimeout
):
GpTcpClientTask
{
    std::make_unique<GpSocketTCP>
    (
        aSocketFlags | GpSocketFlag::NO_BLOCK,
        GpSocket::CloseModeT::CLOSE_ON_DESTRUCT
    )
},
iIOEventPollerIdx{aIOEventPollerIdx},
iRqUP            {std::move(aRequestUP)},
iConnectTimeout  {aConnectTimeout}
{
}

GpHttpClientRequestTask::GpHttpClientRequestTask
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    GpHttpRequest::UP           aRequestUP,
    const milliseconds_t        aConnectTimeout,
    std::string                 aTaskName
):
GpTcpClientTask
{
    std::make_unique<GpSocketTCP>
    (
        aSocketFlags | GpSocketFlag::NO_BLOCK,
        GpSocket::CloseModeT::CLOSE_ON_DESTRUCT
    ),
    std::move(aTaskName)
},
iIOEventPollerIdx{aIOEventPollerIdx},
iRqUP            {std::move(aRequestUP)},
iConnectTimeout  {aConnectTimeout}
{
}

GpHttpClientRequestTask::~GpHttpClientRequestTask (void) noexcept
{
}

void    GpHttpClientRequestTask::OnStart (void)
{
    // Create socket
    GpSocketId socketId = GpSocketId_Default();
    {
        GpUniqueLock uniqueLock{SpinLock()};
        GpSocketTCP& socketTcp = static_cast<GpSocketTCP&>(Socket());
        socketId = socketTcp.Create(GpSocketIPv::IPv4);
    }

    // Register socket and task to IO event poller
    GpIOEventPollerCatalog::SAddSubscription
    (
        socketId,
        SelfWP(),
        iIOEventPollerIdx,
        GpIOEventsTypes
        {
            GpIOEventType::READY_TO_READ,
            GpIOEventType::READY_TO_WRITE,
            GpIOEventType::CLOSED,
            GpIOEventType::ERROR_OCCURRED,
        }
    );

    iProcessState = ProcessStateT::CHECK_CONNECTION;

    // Get server host name to connect
    const GpUrlAuthority& urlAuthority = iRqUP->iRequestNoBody.url.Authority();

    // Connect
    // TODO: implement IPv6
    ConnectTo
    (
        urlAuthority.Host(),
        urlAuthority.Port(),
        GpSocketIPv::IPv4,
        iConnectTimeout,
        SelfWP()
    );

    iSocketTmpBuffer.resize(4096); // TODO: move to config
}

void    GpHttpClientRequestTask::OnStop (ExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        LOG_DEBUG("[GpHttpClientRequestTask::OnStop]:..."_sv);
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

void    GpHttpClientRequestTask::OnReadyToRead (GpSocket& aSocket)
{
    VERIFY
    (
        iProcessState == ProcessStateT::WAIT_FOR_AND_READ_RS,
        "Incorrect HTTP client state detected while incoming data appeared on the socket"_sv
    );

    // Read from socket
    GpByteWriterStorageByteArray    writerStorage(iSocketTmpBuffer);
    GpByteWriter                    writer(writerStorage);

    GpSocketTCP& socketTcp          = static_cast<GpSocketTCP&>(aSocket);
    const size_t readFromSocketSize = socketTcp.Read(writer);

    if (readFromSocketSize == 0) [[unlikely]]
    {
        return;
    }

    if (iHttpParser.ParseNext(GpSpanByteR{std::data(iSocketTmpBuffer), readFromSocketSize}) == true)
    {
        // Get RS
        GpHttpResponse httpRs
        {
            iHttpParser.RsNoBody(),
            iHttpParser.BodyPayload()
        };

        // Fulfill done future
        DonePromise(GpMethodAccess{this}).Fulfill(GpAny{std::move(httpRs)});

        // TODO:
        // If keep-allive, move socket to pool

        // Done task
        std::ignore = RequestStop();
    }
}

void    GpHttpClientRequestTask::OnReadyToWrite (GpSocket& aSocket)
{
    if (iProcessState != ProcessStateT::WRITE_RQ)
    {
        return;
    }

    WriteRqToSocket(aSocket);
}

void    GpHttpClientRequestTask::OnClosed ([[maybe_unused]] GpSocket& aSocket)
{
    LOG_DEBUG("[GpHttpClientRequestTask::OnClosed]"_sv);
}

void    GpHttpClientRequestTask::OnError ([[maybe_unused]] GpSocket& aSocket)
{
    LOG_ERROR("[GpHttpClientRequestTask::OnError]"_sv);
}

void    GpHttpClientRequestTask::OnConnected (GpSocketTCP& aSocket)
{
    std::string_view serverHost = iRqUP->iRequestNoBody.url.Authority().Host();

    LOG_DEBUG
    (
        fmt::format
        (
            "[GpHttpClientRequestTask::OnConnected]: connected to '{}'",
            serverHost
        )
    );

    iProcessState = ProcessStateT::WRITE_RQ;
    WriteRqToSocket(aSocket);
}

void    GpHttpClientRequestTask::OnConnectionTimeout ([[maybe_unused]] GpSocketTCP& aSocket) REQUIRES(SpinLock())
{
    std::string_view serverHost = iRqUP->iRequestNoBody.url.Authority().Host();

    LOG_DEBUG
    (
        fmt::format
        (
            "[GpHttpClientRequestTask::OnConnectionTimeout]: connection to '{}' timed out",
            serverHost
        )
    );

    GpHttpResponse httpRs
    {
        GpHttpResponseNoBodyDesc
        {
            GpHttpResponseCode::REQUEST_TIMEOUT_408
        }
    };

    // Fulfill done future
    DonePromise(GpMethodAccess{this}).Fulfill(GpAny{std::move(httpRs)});
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
            const GpHttpRequest::SerializeRes   serializeRes = GpHttpRequest::SSerialize(*iRqUP, writer);

            iRqBytesToWriteTotal    = writer.TotalWrite();
            iRqBytesWrited          = 0;
            iRqWriteState           = RqWriteStateT::WRITE_HEADERS_S1;

            if (serializeRes == GpHttpRequest::SerializeRes::WRITE_HEADERS_AND_BODY)
            {
                // Write headers and body with single buffer
                iRqUP.reset();
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
            if (iRqUP != nullptr) [[likely]]
            {
                GpHttpBodyPayload::SP& bodySP = iRqUP->iBody;
                if (bodySP.IsNotNULL()) [[likely]]
                {
                    iRqWriteState           = RqWriteStateT::WRITE_BODY_S1;
                    iRqBytesToWriteTotal    = iRqUP->iBody->Size();
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
            const GpSpanByteR rqBodyPayloadFixed = static_cast<const GpHttpBodyPayloadFixed&>(iRqUP->iBody.V()).Data();

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

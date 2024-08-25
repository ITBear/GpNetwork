#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Server/GpHttpServerRequestTask.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReaderStorage.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadFixed.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpRequest.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpResponse.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpHttpServerRequestTask::GpHttpServerRequestTask
(
    GpSocketTCP::SP             aSocketTCP,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    GpHttpRouter::SP            aRouterSP
) noexcept:
GpTcpServerTask
{
    std::move(aSocketTCP),
    aIOEventPollerIdx
},
iRouterSP{std::move(aRouterSP)}
{
}

GpHttpServerRequestTask::~GpHttpServerRequestTask (void) noexcept
{
}

void    GpHttpServerRequestTask::OnStart (void)
{
    GpTcpServerTask::OnStart();

    const GpSocketTCP&  socketTcp   = SocketTCP();
    const GpSocketAddr& remoteAddr  = socketTcp.AddrRemote();
    const GpSocketAddr& localAddr   = socketTcp.AddrLocal();

    LOG_INFO
    (
        fmt::format
        (
            "[GpHttpServerRequestTask::OnStart]: New connection: from {} to {}, socket id {}",
            remoteAddr.ToString(),
            localAddr.ToString(),
            socketTcp.Id()
        )
    );
}

void    GpHttpServerRequestTask::OnStop (StopExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        const GpSocketTCP&  socketTcp   = SocketTCP();
        const GpSocketAddr& remoteAddr  = socketTcp.AddrRemote();
        const GpSocketAddr& localAddr   = socketTcp.AddrLocal();

        LOG_INFO
        (
            fmt::format
            (
                "[GpHttpServerRequestTask::OnStop]: Close connection: from {} to {}, socket id {}",
                remoteAddr.ToString(),
                localAddr.ToString(),
                socketTcp.Id()
            )
        );
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& ex)
    {
        aStopExceptionsOut.emplace_back(GpException{ex.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpHttpServerRequestTask::OnStop]: unknown exception"_sv});
    }

    GpTcpServerTask::OnStop(aStopExceptionsOut);
}

void    GpHttpServerRequestTask::OnStopException (const GpException& aException) noexcept
{
    LOG_EXCEPTION("[GpHttpServerRequestTask::OnStopException]:", aException);
}

void    GpHttpServerRequestTask::OnReadyToRead (GpSocket& aSocket)
{
    try
    {
        if (iProcessState == ProcessStateT::WAIT_FOR_RQ)
        {
            InitCycle();
        }

        if (iProcessState == ProcessStateT::PARSE_RQ)
        {
            // Read from socket
            const size_t readFromSocketSize = SReadFromSocket(aSocket, iSocketTmpBuffer);

            if (readFromSocketSize > 0)
            {
                if (iHttpParser.ParseNext(GpSpanByteR{std::data(iSocketTmpBuffer), readFromSocketSize}) == true)
                {
                    iProcessState = ProcessStateT::RQ_READY;
                }
            }
        }

        if (iProcessState == ProcessStateT::RQ_READY)
        {
            // Try to find handler
            GpHttpRequestHandler::SP requestHandlerSP = iRouterSP.Vn().FindHandler(iHttpParser.RqNoBody());

            iProcessState = ProcessStateT::RS_IN_PROGRESS;
            {
                // Process request
                iRsSP = requestHandlerSP->ProcessRequest
                (
                    MakeSP<GpHttpRequest>
                    (
                        iHttpParser.RqNoBody(),
                        iHttpParser.BodyPayload()
                    )
                );

                // Check 'content-length' header
                {
                    GpHttpHeaders&          headers = iRsSP.Vn().iResponseNoBody.headers;
                    GpHttpBodyPayload::SP&  body    = iRsSP.Vn().iBody;

                    if (body.IsNotNULL()) [[likely]]
                    {
                        headers.SetContentLength(body.Vn().Size());
                    } else
                    {
                        headers.SetContentLength(0);
                    }
                }
            }

            iProcessState = ProcessStateT::WRITE_RS;

            WriteRsToSocket(aSocket);
        }

        if (   (iProcessState == ProcessStateT::RQ_READY)
            || (iProcessState == ProcessStateT::RS_IN_PROGRESS)
            || (iProcessState == ProcessStateT::WRITE_RS))
        {
            GpBytesArray buff;
            const size_t readFromSocketSize = SReadFromSocket(aSocket, buff);

            THROW_COND_GP
            (
                readFromSocketSize == 0,
                "Data income after RQ was full read"
            );
        }
    } catch (const GpHttpException& httpEx)
    {
        iRsSP           = GpHttpResponse::SFromException(httpEx, iHttpParser.RqNoBody().http_version);
        iProcessState   = ProcessStateT::WRITE_RS;

        WriteRsToSocket(aSocket);
    }
}

void    GpHttpServerRequestTask::OnReadyToWrite (GpSocket& aSocket)
{
    WriteRsToSocket(aSocket);
}

void    GpHttpServerRequestTask::OnClosed ([[maybe_unused]] GpSocket& aSocket)
{
    LOG_INFO
    (
        fmt::format
        (
            "[GpHttpServerRequestTask::OnError]: TCP socket closed. Socket id {}",
            aSocket.Id()
        )
    );

    RequestTaskStop();
}

void    GpHttpServerRequestTask::OnError (GpSocket& aSocket)
{
    LOG_ERROR
    (
        fmt::format
        (
            "[GpHttpServerRequestTask::OnError]: TCP socket error. Socket id {}",
            aSocket.Id()
        )
    );

    RequestTaskStop();
}

void    GpHttpServerRequestTask::ProcessOtherMessages (GpAny& aMessage)
{
    LOG_ERROR
    (
        fmt::format
        (
            "[GpHttpServerRequestTask::ProcessOtherMessages]: Get not socket message {}",
            aMessage.TypeInfo().name()
        )
    );
}

void    GpHttpServerRequestTask::InitCycle (void)
{
    iHttpParser.InitCycle(GpHttpParser::Mode::HTTP_REQUEST);

    if (iSocketTmpBuffer.empty())
    {
        iSocketTmpBuffer.resize(4096); // TODO: move to config
    }

    iProcessState = ProcessStateT::PARSE_RQ;
}

void    GpHttpServerRequestTask::FinishCycle (void)
{
    if (iProcessState == ProcessStateT::WAIT_FOR_RQ)
    {
        return;
    }

    // Check if need to close connection
    bool isNeedToCloseConnection = true;
    {
        const GpHttpConnectionFlag::EnumT   rqConnectionFlag = iHttpParser.RqNoBody().headers.GetConnection().value_or(GpHttpConnectionFlag::CLOSE);
        GpHttpConnectionFlag::EnumT         rsConnectionFlag = GpHttpConnectionFlag::KEEP_ALIVE;

        if (iRsSP.IsNotNULL())
        {
            rsConnectionFlag = iRsSP.Vn().iResponseNoBody.headers.GetConnection().value_or(GpHttpConnectionFlag::KEEP_ALIVE);
        }

        isNeedToCloseConnection =
               (rqConnectionFlag == GpHttpConnectionFlag::CLOSE)
            || (rsConnectionFlag == GpHttpConnectionFlag::CLOSE);
    }

    // Clear RQ
    iProcessState = ProcessStateT::WAIT_FOR_RQ;

    // Clear RS
    iRsSP.Clear();
    iRsWriteState = RsWriteStateT::WRITE_HEADERS_S0;

    // Clear parser
    iHttpParser.FinishCycle();

    if (isNeedToCloseConnection)
    {
        RequestTaskStop();
    }
}

size_t  GpHttpServerRequestTask::SReadFromSocket
(
    GpSocket&       aSocket,
    GpBytesArray&   aBufferOut
)
{
    GpByteWriterStorageByteArray    writerStorage(aBufferOut);
    GpByteWriter                    writer(writerStorage);

    GpSocketTCP& socketTcp = static_cast<GpSocketTCP&>(aSocket);
    return socketTcp.Read(writer);
}

void    GpHttpServerRequestTask::WriteRsToSocket (GpSocket& aSocket)
{
    if (iProcessState != ProcessStateT::WRITE_RS)
    {
        return;
    }

    // Write headers
    {
        // Serialize headers
        if (iRsWriteState == RsWriteStateT::WRITE_HEADERS_S0)
        {
            // Serialize RS
            GpByteWriterStorageByteArray        writerStorage(iSocketTmpBuffer);
            GpByteWriter                        writer(writerStorage);
            const GpHttpResponse::SerializeRes  serializeRes = GpHttpResponse::SSerialize(iRsSP.V(), writer);
            iRsBytesToWriteTotal    = writer.TotalWrite();
            iRsBytesWrited          = 0;
            iRsWriteState           = RsWriteStateT::WRITE_HEADERS_S1;

            if (serializeRes == GpHttpResponse::SerializeRes::WRITE_HEADERS_AND_BODY)
            {
                // Write headers and body with single buffer
                iRsSP.Clear();
            }
        }

        // Write headers
        if (iRsWriteState == RsWriteStateT::WRITE_HEADERS_S1)
        {
            GpByteReaderStorage iRsReaderStorage({std::data(iSocketTmpBuffer), iRsBytesToWriteTotal});
            GpByteReader        iRsReader(iRsReaderStorage);
            iRsReader.OffsetAdd(iRsBytesWrited);

            GpSocketTCP& socketTcp  = static_cast<GpSocketTCP&>(aSocket);
            const size_t writeSize  = socketTcp.Write(iRsReader);
            iRsBytesWrited += writeSize;

            if (iRsBytesWrited >= iRsBytesToWriteTotal)
            {
                iRsWriteState = RsWriteStateT::WRITE_BODY_S0;
            }
        }
    }// Write headers

    // Write body
    {
        if (iRsWriteState == RsWriteStateT::WRITE_BODY_S0)
        {
            if (iRsSP.IsNotNULL()) [[likely]]
            {
                GpHttpBodyPayload::SP& bodySP = iRsSP.Vn().iBody;
                if (bodySP.IsNotNULL()) [[likely]]
                {
                    iRsWriteState           = RsWriteStateT::WRITE_BODY_S1;
                    iRsBytesToWriteTotal    = iRsSP.Vn().iBody->Size();
                    iRsBytesWrited          = 0;
                } else
                {
                    iRsWriteState = RsWriteStateT::WRITE_DONE_S0;
                }
            } else
            {
                iRsWriteState = RsWriteStateT::WRITE_DONE_S0;
            }
        }

        if (iRsWriteState == RsWriteStateT::WRITE_BODY_S1)
        {
            // TODO: implement multipart
            const GpSpanByteR rqBodyPayloadFixed = static_cast<const GpHttpBodyPayloadFixed&>(iRsSP.Vn().iBody.V()).Data();

            // -------
            GpByteReaderStorage iRsReaderStorage(rqBodyPayloadFixed);
            GpByteReader        iRsReader(iRsReaderStorage);
            iRsReader.OffsetAdd(iRsBytesWrited);

            GpSocketTCP& socketTcp  = static_cast<GpSocketTCP&>(aSocket);
            const size_t writeSize  = socketTcp.Write(iRsReader);
            iRsBytesWrited += writeSize;

            if (iRsBytesWrited >= rqBodyPayloadFixed.Count())
            {
                iRsWriteState = RsWriteStateT::WRITE_BODY_S0;
            }
        }
    }// Write body

    // Finish
    if (iRsWriteState == RsWriteStateT::WRITE_DONE_S0)
    {
        iRsWriteState = RsWriteStateT::WRITE_DONE_S1;
        FinishCycle();
    }
}

}// namespace GPlatform

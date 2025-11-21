#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpSocketStateTCP)

GpSocketTCP::GpSocketTCP (GpSocketTCP&& aSocket) noexcept:
GpSocket{std::move(aSocket)},
iState{aSocket.iState}
{
}

GpSocketTCP::GpSocketTCP
(
    const GpSocketFlags aFlags,
    const CloseModeT    aCloseMode
) noexcept:
GpSocket
{
    ProtocolT::TCP,
    aFlags,
    aCloseMode
}
{
}

GpSocketTCP::~GpSocketTCP (void) noexcept
{
}

GpSocketTCP&    GpSocketTCP::operator= (GpSocketTCP&& aSocket)
{
    Set(std::move(aSocket));

    return *this;
}

void    GpSocketTCP::Set (GpSocketTCP&& aSocket)
{
    GpSocket::Set(std::move(aSocket));
    iState = aSocket.State();
    aSocket.iState = StateE::NOT_CONNECTED;
}

GpSocketTCP::SP GpSocketTCP::SFromID
(
    GpSocketId          aId,
    const CloseModeT    aCloseMode,
    const StateE        aState
)
{
    GpSocketTCP::SP socketSP = MakeSP<GpSocketTCP>(GpSocketFlags{}, aCloseMode);

    socketSP.V().SetFromRawTCP(aId, aState);

    return socketSP;
}

void    GpSocketTCP::Listen
(
    const GpSocketAddr& aAddr,
    const size_t        aMaxQueueSize
)
{
    try
    {
        VERIFY
        (
            iState == StateE::NOT_CONNECTED,
            "TCP socket state must be NOT_CONNECTED"_sv
        );

        Bind(aAddr);

        const int listenRes = listen(Id(), NumOps::SConvert<int>(aMaxQueueSize));
        SCheckResOrThrow(listenRes, {});

        iState = StateE::LISTEN;
    } catch (...)
    {
        std::exception_ptr currentException = std::current_exception();
        {
            Close();
            iState = StateE::NOT_CONNECTED;
        }

        std::rethrow_exception(currentException);
    }
}

GpSocketTCP::StateE GpSocketTCP::Connect (const GpSocketAddr& aAddr)
{
    try
    {
        VERIFY
        (
            iState == StateE::NOT_CONNECTED,
            "TCP socket state must be NOT_CONNECTED"_sv
        );

        Create(aAddr.IPv());

        if (Flags().Test(GpSocketFlag::NO_BLOCK))
        {
            if (ConnectAsync(aAddr) == StateE::CONNECTION_IN_PROGRESS)
            {
                iState = StateE::CONNECTION_IN_PROGRESS;
                return iState;
            }
        } else
        {
            ConnectSync(aAddr);
            iState = StateE::CONNECTION_IN_PROGRESS;
        }

        OnConnected();
    } catch (...)
    {
        std::exception_ptr currentException = std::current_exception();
        {
            OnConnectionFailed();
        }

        std::rethrow_exception(currentException);
    }

    return iState;
}

void    GpSocketTCP::OnConnected (void)
{
    iState = StateE::OUTGOING;

    SetAddrLocal(GpSocketAddr::SLocalFromSocketId(Id()));
    SetAddrRemote(GpSocketAddr::SRemoteFromSocketId(Id()));
}

void    GpSocketTCP::OnConnectionFailed (void)
{
    Close();
    iState = StateE::NOT_CONNECTED;
}

GpSocketTCP::C::Opts::Val   GpSocketTCP::Accept (const GpSocketFlags& aFlags)
{
    try
    {
        VERIFY
        (
            iState == StateE::LISTEN,
            "TCP socket state must be LISTEN"_sv
        );

        GpSocketId incomingSocketId = accept(Id(), nullptr, nullptr);

        if (incomingSocketId == GpSocketId_Default())
        {
            const int networkErrCode = GpNetworkErrors::SErrno();

            if (GpNetworkErrors::SIsWouldBlock(networkErrCode))
            {
                return std::nullopt;
            }

            SCheckResOrThrow(-1, {});
        }

        GpSocketTCP connectedSocket(aFlags, CloseModeT::CLOSE_ON_DESTRUCT);

        try
        {
            connectedSocket.SetFromRawTCP(incomingSocketId, StateE::INCOMING);
        } catch (...)
        {
            std::exception_ptr currentException = std::current_exception();
            {
                SClose(incomingSocketId);
            }

            std::rethrow_exception(currentException);
        }

        return connectedSocket;
    } catch (...)
    {
        std::exception_ptr currentException = std::current_exception();
        {
            Close();
            iState = StateE::NOT_CONNECTED;
        }

        std::rethrow_exception(currentException);
    }
}

size_t  GpSocketTCP::Read (GpByteWriter& aWriter)
{
    size_t totalRcvSize = 0;

    do
    {
        aWriter.ReserveNext(4096);// TODO: move to socket settings
        GpSpanByteRW writerStoragePtr = aWriter.StoragePtr();

        const ssize_t rcvSize = recv
        (
            Id(),
            reinterpret_cast<char*>(writerStoragePtr.Ptr()),
            NumOps::SConvert<recv_size_t>(writerStoragePtr.Count()),
            0
        );

        if (rcvSize < 0) [[unlikely]]
        {
            const int networkErrCode = GpNetworkErrors::SErrno();

            if (GpNetworkErrors::SIsWouldBlock(networkErrCode))
            {
                return totalRcvSize;
            } else
            {
                SCheckResOrThrow(-1, {});
            }
        } else if (rcvSize == 0)
        {
            break;
        }

        totalRcvSize = NumOps::SAdd(totalRcvSize, size_t(rcvSize));
        aWriter.SubspanThenOffsetAdd(size_t(rcvSize));

        if (size_t(rcvSize) < writerStoragePtr.Count()) [[likely]]
        {
            break;
        }
    } while (true);

    return totalRcvSize;
}

GpByteArray GpSocketTCP::Read (void)
{
    GpByteArray dataBuffer;

    GpByteWriterStorageByteArray    dataWriterStorage(dataBuffer);
    GpByteWriter                    dataWriter(dataWriterStorage);

    std::ignore = Read(dataWriter);

    dataWriter.OnEnd();

    return dataBuffer;
}

size_t  GpSocketTCP::Write (GpByteReader& aReader)
{
    const size_t sizeLeft = aReader.SizeLeft();

    if (sizeLeft == 0)
    {
        return 0;
    }

    GpSpanByteR tryToSendDataPtr = aReader.StoragePtr();

    const ssize_t sendSize = send
    (
        Id(),
        reinterpret_cast<const char*>(tryToSendDataPtr.Ptr()),
        NumOps::SConvert<send_size_t>(tryToSendDataPtr.Count()),
        0
    );

    if (sendSize < 0)
    {
        const int networkErrCode = GpNetworkErrors::SErrno();

        if (GpNetworkErrors::SIsWouldBlock(networkErrCode))
        {
            return 0;
        } else
        {
            SCheckResOrThrow(-1, {});
        }
    }

    aReader.OffsetAdd(size_t(sendSize));

    return size_t(sendSize);
}

size_t  GpSocketTCP::Write (GpSpanByteR aData)
{
    const size_t sizeLeft = aData.SizeInBytes();

    if (sizeLeft == 0)
    {
        return 0;
    }

    const ssize_t sendSize = send
    (
        Id(),
        reinterpret_cast<const char*>(aData.Ptr()),
        NumOps::SConvert<send_size_t>(sizeLeft),
        0
    );

    if (sendSize < 0)
    {
        const int networkErrCode = GpNetworkErrors::SErrno();

        if (GpNetworkErrors::SIsWouldBlock(networkErrCode))
        {
            return 0;
        } else
        {
            SCheckResOrThrow(-1, {});
        }
    }

    return size_t(sendSize);
}

bool    GpSocketTCP::IsConnected (void) const noexcept
{
    return SIsConnected(Id());
}

bool    GpSocketTCP::SIsConnected (const GpSocketId aId) noexcept
{
    char buf = 0;
    const ssize_t rcvRes = recv(aId, &buf, 1, MSG_PEEK);

    if (rcvRes > 0) [[unlikely]]
    {
        return true;
    }

#if defined(GP_POSIX)
    return GpNetworkErrors::SErrno() == EWOULDBLOCK;
#elif defined(GP_OS_WINDOWS)
    return GpNetworkErrors::SErrno() == WSAEWOULDBLOCK;
#else
#   error Unsupported OS
#endif

//#if defined(GP_POSIX)
//  int         error   = 0;
//  socklen_t   len     = sizeof(error);
//  const int   retval  = getsockopt(aId, SOL_SOCKET, SO_ERROR, &error, &len);
//#elif defined(GP_OS_WINDOWS)
//  int         error   = 0;
//  socklen_t   len     = sizeof(error);
//  const int   retval  = getsockopt(aId, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&error), &len);
//#else
//# error Unsupported OS
//#endif

//  return (retval == 0) && (error == 0);
}

void    GpSocketTCP::SetFromRawTCP
(
    const GpSocketId    aId,
    const StateE        aState
)
{
    try
    {
        SetFromRaw(aId);
        iState = aState;
    } catch (...)
    {
        std::exception_ptr currentException = std::current_exception();
        {
            //SetFlag_LingerZero(true);
            Close();
            iState = StateE::NOT_CONNECTED;
        }

        std::rethrow_exception(currentException);
    }
}

void    GpSocketTCP::ConnectSync (const GpSocketAddr& aAddr)
{
    const int res = connect(Id(), aAddr.Raw(), aAddr.RawSize());

    SCheckResOrThrow(res, {});
}

GpSocketTCP::StateE GpSocketTCP::ConnectAsync (const GpSocketAddr& aAddr)
{
    const int res = connect
    (
        Id(),
        aAddr.Raw(),
        aAddr.RawSize()
    );

    if (res < 0)
    {
        const auto networkErrCode = GpNetworkErrors::SErrno();

        if (GpNetworkErrors::SConnInProgress(networkErrCode))
        {
            return StateE::CONNECTION_IN_PROGRESS;
        } else
        {
            SCheckResOrThrow(res, {});
        }
    }

    return StateE::OUTGOING;
}

void    GpSocketTCP::SetUserTimeout ([[maybe_unused]] const milliseconds_t aTimeout)
{
#if defined(GP_OS_LINUX)
    int tcp_timeout = aTimeout.As<int>();

    const int res = setsockopt
    (
        Id(),
        IPPROTO_TCP,
        TCP_USER_TIMEOUT,
        reinterpret_cast<const char*>(&tcp_timeout),
        sizeof(tcp_timeout)
    );

    SCheckResOrThrow(res, {});
#endif
}

void    GpSocketTCP::SetFlag_NoDelay (bool aValue)
{
    int option = aValue ? 1 : 0;
    //const int res = setsockopt(Id(), SOL_TCP, TCP_NODELAY, &option, sizeof(option));
    const int res = setsockopt(Id(), IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&option), sizeof(option));

    SCheckResOrThrow(res, {});
}

}// namespace GPlatform

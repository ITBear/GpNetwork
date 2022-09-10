#include "GpSocketTCP.hpp"

namespace GPlatform {

GpSocketTCP::~GpSocketTCP (void) noexcept
{
}

GpSocketTCP::SP GpSocketTCP::SFromID
(
    GpSocketAddr::SocketIdT aId,
    const CloseModeT        aIsCloseMode,
    const StateT            aState
)
{
    GpSocketTCP::SP socketSP    = MakeSP<GpSocketTCP>(GpSocketFlags{}, aIsCloseMode);
    GpSocketTCP&    socket      = socketSP.V();

    socket.SetFromRawTCP(aId, aState);

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
        THROW_COND_GP
        (
            iState == StateT::NOT_CONNECTED,
            "TCP socket state must be NOT_CONNECTED"_sv
        );

        Bind(aAddr);

        if (listen(Id(), NumOps::SConvert<int>(aMaxQueueSize)) != 0)
        {
            THROW_GP(GpErrno::SGetAndClear());
        }

        iState = StateT::LISTEN;
    } catch (...)
    {
        Close();
        iState = StateT::NOT_CONNECTED;
        throw;
    }
}

void    GpSocketTCP::Connect
(
    const GpSocketAddr&     aAddr,
    const milliseconds_t    aTimeout
)
{
    try
    {
        THROW_COND_GP
        (
            iState == StateT::NOT_CONNECTED,
            "TCP socket state must be NOT_CONNECTED"_sv
        );

        Create(aAddr.IPv());
        SetUserTimeout(aTimeout);

        iState = StateT::CONNECTION_IN_PROGRESS;

        if (Flags().Test(GpSocketFlag::NO_BLOCK))
        {
            ConnectAsync(aAddr);
        } else
        {
            ConnectSync(aAddr);
        }

        SetUserTimeout(0.0_si_ms);

        iState = StateT::OUTGOING;

        AddrLocal().LocalFromSocketId(Id());
        AddrRemote().RemoteFromSocketId(Id());
    } catch (...)
    {
        Close();
        iState = StateT::NOT_CONNECTED;
        throw;
    }
}

GpSocketTCP::SP GpSocketTCP::Accept (const GpSocketFlags& aFlags)
{
    try
    {
        THROW_COND_GP
        (
            iState == StateT::LISTEN,
            "TCP socket state must be LISTEN"_sv
        );

        const GpSocketAddr::SocketIdT incomingSocketId = accept(Id(), nullptr, nullptr);

        if (incomingSocketId == GpSocketAddr::sDefaultSocketId)
        {
            if (errno == EAGAIN)
            {
                return GpSocketTCP::SP::SNull();
            }

            THROW_GP(GpErrno::SGetAndClear());
        }

        GpSocketTCP::SP connectedSocket = GpSocketTCP::SP::SNew(aFlags, CloseModeT::CLOSE_ON_DESTRUCT);

        try
        {
            connectedSocket.Vn().SetFromRawTCP(incomingSocketId, StateT::INCOMING);
        } catch (...)
        {
            close(incomingSocketId);
            throw;
        }

        return connectedSocket;
    } catch (...)
    {
        Close();
        iState = StateT::NOT_CONNECTED;
        throw;
    }
}

size_t  GpSocketTCP::Read (GpByteWriter& aWriter)
{
    ssize_t rcvSize         = 0;
    size_t  totalRcvSize    = 0;

    do
    {
        aWriter.ReserveNext(8192);//TODO: move to socket settings
        GpSpanPtrByteRW writerStoragePtr = aWriter.StoragePtr();

        rcvSize = recv(Id(), writerStoragePtr.Ptr(), writerStoragePtr.Count(), 0);

        if (rcvSize < 0)
        {
GP_WARNING_PUSH()
GP_WARNING_DISABLE(logical-op)
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
GP_WARNING_POP()
            {
                return totalRcvSize;
            } else
            {
                THROW_GP(GpErrno::SGetAndClear());
            }
        } else if (rcvSize == 0)
        {
            break;
        }

        totalRcvSize = NumOps::SAdd(totalRcvSize, size_t(rcvSize));
        aWriter.Offset(size_t(rcvSize));

        if (size_t(rcvSize) < writerStoragePtr.Count())
        {
            break;
        }
    } while (true);

    return totalRcvSize;
}

size_t  GpSocketTCP::Write (GpByteReader& aReader)
{
    const size_t sizeLeft = aReader.SizeLeft();

    if (sizeLeft == 0)
    {
        return 0;
    }

    GpSpanPtrByteR  tryToSendDataPtr    = aReader.StoragePtr();
    ssize_t         sendSize            = send(Id(), tryToSendDataPtr.Ptr(), tryToSendDataPtr.Count(), 0);

    if (sendSize < 0)
    {
GP_WARNING_PUSH()
GP_WARNING_DISABLE(logical-op)
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return 0;
        } else
        {
            THROW_GP(GpErrno::SGetAndClear());
        }
    }

    aReader.Offset(size_t(sendSize));

    return size_t(sendSize);
}

void    GpSocketTCP::SetFromRawTCP
(
    const GpSocketAddr::SocketIdT   aId,
    const StateT                    aState
)
{
    try
    {
        SetFromRaw(aId);
        iState = aState;
    } catch (...)
    {
//      SetFlag_LingerZero(true);
        Close();
        iState = StateT::NOT_CONNECTED;
        throw;
    }
}

void    GpSocketTCP::ConnectSync (const GpSocketAddr& aAddr)
{
    if (connect(Id(), aAddr.Raw_sockaddr(), aAddr.Raw_sockaddrSize()) < 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }
}

void    GpSocketTCP::ConnectAsync (const GpSocketAddr& aAddr)
{
    if (connect(Id(), aAddr.Raw_sockaddr(), aAddr.Raw_sockaddrSize()) < 0)
    {
        THROW_GP(GpErrno::SGetAndClear());

        if (errno == EINPROGRESS)
        {
            THROW_COND_GP
            (
                GpTaskFiber::SIsIntoFiber(),
                "NO_BLOCK mode available only from inside fiber task"_sv
            );

            //Wait
            YELD_WAITING();

            //TODO: implement event processing
            THROW_GP_NOT_IMPLEMENTED();
            /*//Check result
            const GpIODeviceEvents& ioEvents = GpTaskCoroutineCtx::STaskSignal<GpIODeviceSignal>().Vn().Events();

            if (ioEvents.TestFlagE(GpIODeviceEvent::CLOSED) ||
                ioEvents.TestFlagE(GpIODeviceEvent::ERROR_OCCURRED))
            {
                THROW_GP_EXCEPTION("Failed to connect to "_sv + aAddr.ToString());
            }*/
        } else
        {

        }
    }
}

void    GpSocketTCP::SetUserTimeout (const milliseconds_t aTimeout)
{
    int tcp_timeout = aTimeout.As<int>();

    if (setsockopt(Id(), IPPROTO_TCP, TCP_USER_TIMEOUT, &tcp_timeout, sizeof(tcp_timeout)) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }
}

}//namespace GPlatform

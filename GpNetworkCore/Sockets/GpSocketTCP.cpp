#include "GpSocketTCP.hpp"

#include <GpCore2/GpUtils/Streams/GpByteWriter.cpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>

namespace GPlatform {

GpSocketTCP::~GpSocketTCP (void) noexcept
{
}

GpSocketTCP::SP GpSocketTCP::SFromID
(
    GpSocketId          aId,
    const CloseModeT    aCloseMode,
    const StateT        aState
)
{
    GpSocketTCP::SP socketSP    = MakeSP<GpSocketTCP>(GpSocketFlags{}, aCloseMode);
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

        const int listenRes = listen(Id(), NumOps::SConvert<int>(aMaxQueueSize));
        SCheckResOrThrow(listenRes, {});

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

        SetAddrLocal(GpSocketAddr::SLocalFromSocketId(Id()));
        SetAddrRemote(GpSocketAddr::SRemoteFromSocketId(Id()));
    } catch (...)
    {
        Close();
        iState = StateT::NOT_CONNECTED;
        throw;
    }
}

GpSocketTCP::C::Opt::Val    GpSocketTCP::Accept (const GpSocketFlags& aFlags)
{
    try
    {
        THROW_COND_GP
        (
            iState == StateT::LISTEN,
            "TCP socket state must be LISTEN"_sv
        );

        GpSocketId incomingSocketId = accept(Id(), nullptr, nullptr);

        if (incomingSocketId == GpSocketId_Default())
        {
            if (errno == EAGAIN)
            {
                return std::nullopt;
            }

            SCheckResOrThrow(-1, {});
        }

        GpSocketTCP connectedSocket(aFlags, CloseModeT::CLOSE_ON_DESTRUCT);

        try
        {
            connectedSocket.SetFromRawTCP(incomingSocketId, StateT::INCOMING);
        } catch (...)
        {
            SClose(incomingSocketId);
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
    size_t  totalRcvSize    = 0;

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
GP_WARNING_PUSH()
#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(unknown-warning-option)
    GP_WARNING_DISABLE(logical-op)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
            if (   (errno == EAGAIN)
                || (errno == EWOULDBLOCK))
GP_WARNING_POP()
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
        aWriter.OffsetAdd(size_t(rcvSize));

        if (size_t(rcvSize) < writerStoragePtr.Count()) [[likely]]
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

    GpSpanByteR tryToSendDataPtr    = aReader.StoragePtr();
    ssize_t     sendSize            = send
    (
        Id(),
        reinterpret_cast<const char*>(tryToSendDataPtr.Ptr()),
        NumOps::SConvert<send_size_t>(tryToSendDataPtr.Count()),
        0
    );

    if (sendSize < 0)
    {
GP_WARNING_PUSH()
#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(unknown-warning-option)
    GP_WARNING_DISABLE(logical-op)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
GP_WARNING_POP()
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

void    GpSocketTCP::SetFromRawTCP
(
    const GpSocketId    aId,
    const StateT        aState
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
    const int res = connect(Id(), aAddr.Raw(), aAddr.RawSize());
    SCheckResOrThrow(res, {});
}

void    GpSocketTCP::ConnectAsync (const GpSocketAddr& aAddr)
{
    const int res = connect
    (
        Id(),
        aAddr.Raw(),
        aAddr.RawSize()
    );

    if (res < 0)
    {
        //TODO: implement
        THROW_GP_NOT_IMPLEMENTED();

        //THROW_GP(GpErrno::SGetAndClear());

        /*if (errno == EINPROGRESS)
        {
            THROW_COND_GP
            (
                GpTaskFiber::SIsIntoFiber(),
                "NO_BLOCK mode available only from inside fiber task"_sv
            );

            //Wait
            ?
            YELD_WAIT();

            //TODO: implement event processing
            THROW_GP_NOT_IMPLEMENTED();
            //Check result
            const GpIODeviceEvents& ioEvents = GpTaskCoroutineCtx::STaskSignal<GpIODeviceSignal>().Vn().Events();

            if (ioEvents.TestFlagE(GpIODeviceEvent::CLOSED) ||
                ioEvents.TestFlagE(GpIODeviceEvent::ERROR_OCCURRED))
            {
                THROW_GP("Failed to connect to "_sv + aAddr.ToString());
            }
        } else
        {

        }*/
    }
}

void    GpSocketTCP::SetUserTimeout ([[maybe_unused]] const milliseconds_t aTimeout)
{
#if defined(GP_POSIX)
    int tcp_timeout = aTimeout.As<int>();
    const int res = setsockopt(Id(), IPPROTO_TCP, TCP_USER_TIMEOUT, reinterpret_cast<const char*>(&tcp_timeout), sizeof(tcp_timeout));

    SCheckResOrThrow(res, {});
#endif
}

}// namespace GPlatform

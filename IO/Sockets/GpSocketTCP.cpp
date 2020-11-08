#include "GpSocketTCP.hpp"

namespace GPlatform {

GpSocketTCP::GpSocketTCP (const GpSocketFlags& aFlags) noexcept:
GpSocket(ProtocolT::TCP, aFlags)
{
}

GpSocketTCP::~GpSocketTCP (void) noexcept
{
}

void    GpSocketTCP::Listen (const GpSocketAddr&    aAddr,
                             const count_t          aMaxQueueSize)
{
    try
    {
        THROW_GPE_COND_CHECK_M(iState == StateT::NOT_CONNECTED,
                               "TCP socket state must be NOT_CONNECTED"_sv);

        Bind(aAddr);

        if (listen(Id(), int(aMaxQueueSize.Value())) != 0)
        {
            THROW_GPE(GpErrno::SGetAndClear());
        }

        iState = StateT::LISTEN;
    } catch (...)
    {
        Close();
        iState = StateT::NOT_CONNECTED;
        throw;
    }
}

void    GpSocketTCP::Connect (const GpSocketAddr&   aAddr,
                              const milliseconds_t  aTimeout)
{
    try
    {
        THROW_GPE_COND_CHECK_M(iState == StateT::NOT_CONNECTED,
                               "TCP socket state must be NOT_CONNECTED"_sv);

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
        THROW_GPE_COND_CHECK_M(iState == StateT::LISTEN,
                               "TCP socket state must be LISTEN"_sv);

        const GpSocketAddr::SocketIdT incomingSocketId = accept(Id(), nullptr, nullptr);

        if (incomingSocketId == EAGAIN)
        {
            return GpSocketTCP::SP::SNull();
        }

        if (incomingSocketId == GpSocketAddr::sDefaultSocketId)
        {
            THROW_GPE(GpErrno::SGetAndClear());
        }

        GpSocketTCP::SP connectedSocket = GpSocketTCP::SP::SNew(aFlags);

        try
        {
            connectedSocket.Vn().SetFromIncomingRawId(incomingSocketId);
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

size_byte_t GpSocketTCP::Read (GpByteWriter& aWriter)
{
    std::array<std::byte, 8192> buffer;
    ssize_t                     rcvSize;
    size_byte_t                 totalRcvSize = 0_byte;

    while ((rcvSize = recv(Id(), buffer.data(), buffer.size(), 0)) > 0)
    {
        totalRcvSize += size_byte_t::SMake(size_t(rcvSize));
        aWriter.Bytes({buffer.data(), size_t(rcvSize)});
    }

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
            THROW_GPE(GpErrno::SGetAndClear());
        }
    }

    return totalRcvSize;
}

size_byte_t GpSocketTCP::Write (GpByteReader& aReader)
{
    const size_byte_t sizeLeft = aReader.SizeLeft();

    if (sizeLeft == 0_byte)
    {
        return 0_byte;
    }

    GpRawPtrByteR dataPtr = aReader.Bytes(sizeLeft);
    ssize_t sndSize = send(Id(), dataPtr.PtrBegin(), dataPtr.SizeLeft().As<size_t>(), 0);

    if (sndSize < 0)
    {
GP_WARNING_PUSH()
GP_WARNING_DISABLE(logical-op)
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return 0_byte;
        } else
        {
            THROW_GPE(GpErrno::SGetAndClear());
        }
    }

    const size_byte_t sizeWrite = size_byte_t::SMake(size_t(sndSize));

    if (sizeWrite != sizeLeft)
    {
        aReader.Rollback(sizeLeft - sizeWrite);
    }

    return sizeWrite;
}

void    GpSocketTCP::SetFromIncomingRawId (const GpSocketAddr::SocketIdT aId)
{
    try
    {
        THROW_GPE_COND_CHECK_M(iState == StateT::NOT_CONNECTED,
                               "TCP socket state must be NOT_CONNECTED"_sv);

        SetFromRaw(aId);
        iState = StateT::INCOMING;
    } catch (...)
    {
        Close();
        iState = StateT::NOT_CONNECTED;
        throw;
    }
}

void    GpSocketTCP::ConnectSync (const GpSocketAddr& aAddr)
{
    if (connect(Id(), aAddr.Raw_sockaddr(), aAddr.Raw_sockaddrSize()) < 0)
    {
        THROW_GPE(GpErrno::SGetAndClear());
    }
}

void    GpSocketTCP::ConnectAsync (const GpSocketAddr& aAddr)
{
    if (connect(Id(), aAddr.Raw_sockaddr(), aAddr.Raw_sockaddrSize()) < 0)
    {
        THROW_GPE(GpErrno::SGetAndClear());

        if (errno == EINPROGRESS)
        {
            THROW_GPE_COND_CHECK_M(GpTaskFiberCtx::SIsIntoFiber(), "NO_BLOCK mode available only from inside fiber task"_sv);

            //Wait
            GpTaskFiberCtx::SYeld(GpTask::ResT::WAITING);

            //TODO: implement event processing
            THROW_NOT_IMPLEMENTED();
            /*//Check result
            const GpIODeviceEvents& ioEvents = GpTaskCoroutineCtx::STaskSignal<GpIODeviceSignal>().VCn().Events();

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
        THROW_GPE(GpErrno::SGetAndClear());
    }
}

}//namespace GPlatform

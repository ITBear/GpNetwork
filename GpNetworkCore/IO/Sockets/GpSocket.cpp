#include "GpSocket.hpp"

namespace GPlatform {

GpSocket::~GpSocket (void) noexcept
{
    if (iCloseMode == CloseModeT::CLOSE_ON_DESTRUCT)
    {
        Close();
    }
}

void    GpSocket::WriteAll (GpSpanPtrByteR aData)
{
    GpByteReaderStorage readerStorage(aData);
    GpByteReader        reader(readerStorage);

    while (reader.SizeLeft() > 0)
    {
        Write(reader);
    }
}

void    GpSocket::CheckForErrors (void) const
{
    int       error = 0;
    socklen_t errlen = sizeof(error);
    if (getsockopt(iId, SOL_SOCKET, SO_ERROR, static_cast<void*>(&error), &errlen) == 0)
    {
        THROW_GP(strerror(error));
    }
}

void    GpSocket::Bind (const GpSocketAddr& aAddr)
{
    Create(aAddr.IPv());

    //Bind socket
    if (bind(Id(),
        aAddr.Raw_sockaddr(),
        aAddr.Raw_sockaddrSize()) < 0)
    {
        Close();
        THROW_GP(GpErrno::SGetAndClear());
    }

    iAddrLocal.LocalFromSocketId(Id());
}

void    GpSocket::Close (void) noexcept
{
    iAddrLocal.Clear();
    iAddrRemote.Clear();

    if (Id() != GpSocketAddr::sDefaultSocketId)
    {
        close(Id());

        iId = GpSocketAddr::sDefaultSocketId;
    }
}

void    GpSocket::Create (IPvTE aIPv)
{
    if (Id() != GpSocketAddr::sDefaultSocketId)
    {
        Close();
    }

    iIPv = aIPv;

    const ProtocolTE protocol = Protocol();

    GpSocketAddr::SocketIdT socketID = socket(GpSocketIPv_SSFamily(iIPv),
                                              int(GpSocketProtocol_Type(protocol)),
                                              int(GpSocketProtocol_Proto(protocol)));

    if (socketID == GpSocketAddr::sDefaultSocketId)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }

    iId = socketID;

    ApplyFlags();
}

void    GpSocket::SetFromRaw (const GpSocketAddr::SocketIdT aId)
{
    if (Id() != GpSocketAddr::sDefaultSocketId)
    {
        Close();
    }

    iId = aId;

    AddrLocal().LocalFromSocketId(Id());
    AddrRemote().RemoteFromSocketId(Id());

    iIPv = AddrLocal().IPv();

    ApplyFlags();
}

void    GpSocket::ApplyFlags (void)
{
    if (iFlags.Test(GpSocketFlag::REUSE_ADDR))
    {
        SetFlag_ReuseAddr(true);
    }

    if (iFlags.Test(GpSocketFlag::REUSE_PORT))
    {
        SetFlag_ReusePort(true);
    }

    if (iFlags.Test(GpSocketFlag::NO_BLOCK))
    {
        SetFlag_NoBlock(true);
    }

    if (iFlags.Test(GpSocketFlag::LINGER_ZERO))
    {
        SetFlag_LingerZero(true);
    }

    if (iFlags.Test(GpSocketFlag::NO_DELAY))
    {
        SetFlag_NoDelay(true);
    }
}

void    GpSocket::SetFlag_ReuseAddr (bool aValue)
{
    int option = aValue ? 1 : 0;
    if (setsockopt(Id(), SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }
}

void    GpSocket::SetFlag_ReusePort (bool aValue)
{
    int option = aValue ? 1 : 0;
    if (setsockopt(Id(), SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }
}

void    GpSocket::SetFlag_NoBlock (bool aValue)
{
    int opts = fcntl(Id(), F_GETFL);

    THROW_COND_GP
    (
        opts >= 0,
        [&](){return GpErrno::SGetAndClear();}
    );

    if (aValue) opts = BitOps::Up(opts, O_NONBLOCK);
    else        opts = BitOps::Down(opts, O_NONBLOCK);

    THROW_COND_GP
    (
        fcntl(Id(), F_SETFL, opts) >= 0,
        [&](){return GpErrno::SGetAndClear();}
    );
}

void    GpSocket::SetFlag_LingerZero (bool aValue)
{
    struct linger sl;
    sl.l_onoff  = aValue ? 1 : 0;
    sl.l_linger = 0;
    if (setsockopt(Id(), SOL_SOCKET, SO_LINGER, &sl, sizeof(sl)) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }
}

void    GpSocket::SetFlag_NoDelay (bool aValue)
{
    int option = aValue ? 1 : 0;
    if (setsockopt(Id(), SOL_TCP, TCP_NODELAY, &option, sizeof(option)) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }
}

}//namespace GPlatform

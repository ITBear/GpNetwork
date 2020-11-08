#include "GpSocket.hpp"
#include <iostream>

namespace GPlatform {

GpSocket::GpSocket (const ProtocolTE        aProtocol,
                    const GpSocketFlags&    aFlags) noexcept:
iProtocol(aProtocol),
iFlags(aFlags)
{
    std::cout << "[GpSocket::GpSocket]:..." << std::endl;
}

GpSocket::~GpSocket (void) noexcept
{
    std::cout << "[GpSocket::~GpSocket]:..." << std::endl;

    Close();
}

void    GpSocket::WriteAll (GpRawPtrByteR aData)
{
    GpByteReaderStorage readerStorage(aData);
    GpByteReader        reader(readerStorage);
    while (reader.SizeLeft() > 0_byte)
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
        THROW_GPE(strerror(error));
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
        THROW_GPE(GpErrno::SGetAndClear());
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
        THROW_GPE(GpErrno::SGetAndClear());
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
}

void    GpSocket::SetFlag_ReuseAddr (bool aValue)
{
    int option = aValue ? 1 : 0;
    if (setsockopt(Id(), SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) != 0)
    {
        THROW_GPE(GpErrno::SGetAndClear());
    }
}

void    GpSocket::SetFlag_ReusePort (bool aValue)
{
    int option = aValue ? 1 : 0;
    if (setsockopt(Id(), SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) != 0)
    {
        THROW_GPE(GpErrno::SGetAndClear());
    }
}

void    GpSocket::SetFlag_NoBlock (bool aValue)
{
    int opts = fcntl(Id(), F_GETFL);
    THROW_GPE_COND_CHECK_M(opts >= 0, GpErrno::SGetAndClear());

    if (aValue) opts = BitOps::Up(opts, O_NONBLOCK);
    else        opts = BitOps::Down(opts, O_NONBLOCK);

    THROW_GPE_COND_CHECK_M(fcntl(Id(), F_SETFL, opts) >= 0, GpErrno::SGetAndClear());
}

}//namespace GPlatform
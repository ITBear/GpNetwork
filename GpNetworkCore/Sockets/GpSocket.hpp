#pragma once

#include "../GpIOObjectId.hpp"
#include "GpSocketProtocol.hpp"
#include "GpSocketFlags.hpp"
#include "GpSocketAddr.hpp"

#include <sys/socket.h>
#include <netinet/in.h>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocket
{
public:
    CLASS_REMOVE_CTRS_COPY(GpSocket)
    CLASS_DD(GpSocket)

    using ProtocolT     = GpSocketProtocol;
    using ProtocolTE    = ProtocolT::EnumT;
    using IPvT          = GpSocketIPv;
    using IPvTE         = IPvT::EnumT;

    enum class CloseModeT
    {
        KEEP_ON_DESTRUCT,
        CLOSE_ON_DESTRUCT
    };

protected:
    inline                  GpSocket            (void) noexcept;
    inline                  GpSocket            (GpSocket&& aSocket) noexcept;
    inline                  GpSocket            (const ProtocolTE       aProtocol,
                                                 const GpSocketFlags&   aFlags,
                                                 const CloseModeT       aCloseMode) noexcept;

    inline                  GpSocket            (const GpIOObjectId     aId,
                                                 const IPvTE            aIpV,
                                                 const ProtocolTE       aProtocol,
                                                 const GpSocketFlags&   aFlags,
                                                 const CloseModeT       aCloseMode) noexcept;

public:
    virtual                 ~GpSocket           (void) noexcept;

    ProtocolTE              Protocol            (void) const noexcept {return iProtocol;}
    GpIOObjectId            Id                  (void) const noexcept {return iId;}
    bool                    IsValidId           (void) const noexcept {return iId != GpIOObjectId_Default();}
    IPvTE                   IPv                 (void) const noexcept {return iIPv;}
    const GpSocketAddr&     AddrLocal           (void) const noexcept {return iAddrLocal;}
    const GpSocketAddr&     AddrRemote          (void) const noexcept {return iAddrRemote;}
    const GpSocketFlags&    Flags               (void) const noexcept {return iFlags;}
    CloseModeT              CloseMode           (void) const noexcept {return iCloseMode;}

    inline void             CheckForErrors      (void) const;
    inline void             Close               (void);
    inline void             Bind                (const GpSocketAddr& aAddr);

protected:
    GpSocket&               operator=           (GpSocket&&) noexcept = delete;
    inline void             Set                 (GpSocket&& aSocket);
    inline void             Create              (IPvTE aIPv);
    inline void             SetAddrLocal        (const GpSocketAddr& aAddr) noexcept {iAddrLocal = aAddr;}
    inline void             SetAddrRemote       (const GpSocketAddr& aAddr) noexcept {iAddrRemote = aAddr;}

    inline void             SetFromRaw          (const GpIOObjectId aId);

protected:
    inline void             ApplyFlags          (void);
    inline void             SetFlag_ReuseAddr   (bool aValue);
    inline void             SetFlag_ReusePort   (bool aValue);
    inline void             SetFlag_NoBlock     (bool aValue);
    inline void             SetFlag_LingerZero  (bool aValue);
    inline void             SetFlag_RecvPktInfo (bool aValue);

private:
    GpIOObjectId            iId         = GpIOObjectId_Default();
    IPvTE                   iIPv        = IPvTE::IPv4;
    ProtocolTE              iProtocol;
    GpSocketAddr            iAddrLocal;
    GpSocketAddr            iAddrRemote;
    GpSocketFlags           iFlags;
    CloseModeT              iCloseMode  = CloseModeT::CLOSE_ON_DESTRUCT;
};

GpSocket::GpSocket (void) noexcept
{
}

GpSocket::GpSocket (GpSocket&& aSocket) noexcept:
iId        (std::move(aSocket.iId        )),
iIPv       (std::move(aSocket.iIPv       )),
iProtocol  (std::move(aSocket.iProtocol  )),
iAddrLocal (std::move(aSocket.iAddrLocal )),
iAddrRemote(std::move(aSocket.iAddrRemote)),
iFlags     (std::move(aSocket.iFlags     )),
iCloseMode (std::move(aSocket.iCloseMode ))
{
    //aSocket
    aSocket.iId         = GpIOObjectId_Default();
    aSocket.iCloseMode  = CloseModeT::KEEP_ON_DESTRUCT;
}

GpSocket::GpSocket
(
    const ProtocolTE        aProtocol,
    const GpSocketFlags&    aFlags,
    const CloseModeT        aCloseMode
) noexcept:
iProtocol (aProtocol),
iFlags    (aFlags),
iCloseMode(aCloseMode)
{
}

GpSocket::GpSocket
(
    const GpIOObjectId      aId,
    const IPvTE             aIpV,
    const ProtocolTE        aProtocol,
    const GpSocketFlags&    aFlags,
    const CloseModeT        aCloseMode
) noexcept:
iId       (aId),
iIPv      (aIpV),
iProtocol (aProtocol),
iFlags    (aFlags),
iCloseMode(aCloseMode)
{
}

void    GpSocket::CheckForErrors (void) const
{
    int       error     = 0;
    socklen_t errlen    = sizeof(error);
    const int res       = getsockopt
    (
        iId,
        SOL_SOCKET,
        SO_ERROR,
        &error,
        &errlen
    );

    THROW_COND_GP
    (
        res == 0,
        [&error](){return strerror(error);}
    );
}

void    GpSocket::Close (void)
{
    iAddrLocal.Clear();
    iAddrRemote.Clear();

    if (!IsValidId())
    {
        return;
    }

    if (iCloseMode != CloseModeT::CLOSE_ON_DESTRUCT)
    {
        iId = GpIOObjectId_Default();
        return;
    }

    const int res = close(Id());
    iId = GpIOObjectId_Default();

    THROW_COND_GP
    (
        res == 0,
        []()
        {
            return std::u8string(GpErrno::SGetAndClear());
        }
    );
}

void    GpSocket::Bind (const GpSocketAddr& aAddr)
{
    Create(aAddr.IPv());

    const int res = bind
    (
        Id(),
        aAddr.Raw(),
        aAddr.RawSize()
    );

    if (res != 0)
    {
        Close();
        THROW_GP(GpErrno::SGetAndClear());
    }

    iAddrLocal = GpSocketAddr::SLocalFromSocketId(Id());
    iAddrRemote.Clear();
}

void    GpSocket::Set (GpSocket&& aSocket)
{
    if (this == &aSocket)
    {
        return;
    }

    Close();

    iId         = std::move(aSocket.iId        );
    iIPv        = std::move(aSocket.iIPv       );
    iProtocol   = std::move(aSocket.iProtocol  );
    iAddrLocal  = std::move(aSocket.iAddrLocal );
    iAddrRemote = std::move(aSocket.iAddrRemote);
    iFlags      = std::move(aSocket.iFlags     );
    iCloseMode  = std::move(aSocket.iCloseMode );

    //aSocket
    aSocket.iId         = GpIOObjectId_Default();
    aSocket.iCloseMode  = CloseModeT::KEEP_ON_DESTRUCT;
}

void    GpSocket::Create (IPvTE aIPv)
{
    if (Id() != GpIOObjectId_Default())
    {
        Close();
    }

    iIPv = aIPv;

    const ProtocolTE    protocol = Protocol();
    const GpIOObjectId  socketID = socket
    (
        GpSocketIPv_SSFamily(iIPv),
        int(GpSocketProtocol_Type(protocol)),
        int(GpSocketProtocol_Proto(protocol))
    );

    THROW_COND_GP
    (
        socketID != GpIOObjectId_Default(),
        [](){return std::u8string(GpErrno::SGetAndClear());}
    );

    iId = socketID;

    ApplyFlags();
}

void    GpSocket::SetFromRaw (const GpIOObjectId aId)
{
    if (Id() != GpIOObjectId_Default())
    {
        Close();
    }

    iId         = aId;
    iAddrLocal  = GpSocketAddr::SLocalFromSocketId(Id());
    iAddrRemote = GpSocketAddr::SRemoteFromSocketId(Id());
    iIPv        = AddrLocal().IPv();

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

    if (iFlags.Test(GpSocketFlag::RECV_PKT_INFO))
    {
        SetFlag_RecvPktInfo(true);
    }

    //SetFlag_NoDelay (iFlags.Test(GpSocketFlag::NO_DELAY));
}

void    GpSocket::SetFlag_ReuseAddr (bool aValue)
{
    int option = aValue ? 1 : 0;

    THROW_COND_GP
    (
        setsockopt(Id(), SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == 0,
        []()
        {
            return std::u8string(GpErrno::SGetAndClear());
        }
    );
}

void    GpSocket::SetFlag_ReusePort (bool aValue)
{
    int option = aValue ? 1 : 0;

    THROW_COND_GP
    (
        setsockopt(Id(), SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) == 0,
        []()
        {
            return std::u8string(GpErrno::SGetAndClear());
        }
    );
}

void    GpSocket::SetFlag_NoBlock (bool aValue)
{
    int opts = fcntl(Id(), F_GETFL);

    THROW_COND_GP
    (
        opts >= 0,
        []()
        {
            return std::u8string(GpErrno::SGetAndClear());
        }
    );

    if (aValue)
    {
        opts = BitOps::Up_by_mask(opts, O_NONBLOCK);
    } else
    {
        opts = BitOps::Down_by_mask(opts, O_NONBLOCK);
    }

    THROW_COND_GP
    (
        fcntl(Id(), F_SETFL, opts) >= 0,
        []()
        {
            return std::u8string(GpErrno::SGetAndClear());
        }
    );
}

void    GpSocket::SetFlag_LingerZero (bool aValue)
{
    struct linger sl;
    sl.l_onoff  = aValue ? 1 : 0;
    sl.l_linger = 0;

    THROW_COND_GP
    (
        setsockopt(Id(), SOL_SOCKET, SO_LINGER, &sl, sizeof(sl)) == 0,
        []()
        {
            return std::u8string(GpErrno::SGetAndClear());
        }
    );
}

void    GpSocket::SetFlag_RecvPktInfo (bool aValue)
{
    int enable  = aValue ? 1 : 0;
    int level   = iIPv == IPvTE::IPv4 ? IPPROTO_IP :IPPROTO_IPV6;

    THROW_COND_GP
    (
        setsockopt(Id(), level, IP_PKTINFO, &enable, sizeof(enable)) == 0,
        []()
        {
            return std::u8string(GpErrno::SGetAndClear());
        }
    );
}

}// namespace GPlatform

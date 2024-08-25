#pragma once

#include "../GpNetworkErrors.hpp"
#include "GpSocketProtocol.hpp"
#include "GpSocketFlags.hpp"
#include "GpSocketAddr.hpp"

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
    inline                  GpSocket            (ProtocolTE     aProtocol,
                                                 GpSocketFlags  aFlags,
                                                 CloseModeT     aCloseMode) noexcept;

    inline                  GpSocket            (GpSocketId     aId,
                                                 IPvTE          aIpV,
                                                 ProtocolTE     aProtocol,
                                                 GpSocketFlags  aFlags,
                                                 CloseModeT     aCloseMode) noexcept;

public:
    virtual                 ~GpSocket           (void) noexcept;

    ProtocolTE              Protocol            (void) const noexcept {return iProtocol;}
    GpSocketId              Id                  (void) const noexcept {return iId;}
    bool                    IsValidId           (void) const noexcept {return iId != GpSocketId_Default();}
    IPvTE                   IPv                 (void) const noexcept {return iIPv;}
    const GpSocketAddr&     AddrLocal           (void) const noexcept {return iAddrLocal;}
    const GpSocketAddr&     AddrRemote          (void) const noexcept {return iAddrRemote;}
    const GpSocketFlags&    Flags               (void) const noexcept {return iFlags;}
    CloseModeT              CloseMode           (void) const noexcept {return iCloseMode;}

    inline void             CheckForErrors      (void) const;
    inline void             Close               (void);
    inline void             Bind                (const GpSocketAddr& aAddr);
    inline void             Create              (IPvTE aIPv);

protected:
    GpSocket&               operator=           (GpSocket&&) noexcept = delete;
    inline void             Set                 (GpSocket&& aSocket);
    inline void             SetAddrLocal        (const GpSocketAddr& aAddr) noexcept {iAddrLocal = aAddr;}
    inline void             SetAddrRemote       (const GpSocketAddr& aAddr) noexcept {iAddrRemote = aAddr;}

    inline void             SetFromRaw          (const GpSocketId aId);

protected:
    inline void             ApplyFlags          (void);
    inline void             SetFlag_ReuseAddr   (bool aValue);
    inline void             SetFlag_NoBlock     (bool aValue);
    inline void             SetFlag_LingerZero  (bool aValue);
    inline void             SetFlag_RecvPktInfo (bool aValue);

#if defined(GP_POSIX)
    inline void             SetFlag_ReusePort   (bool aValue);
#endif// #if defined(GP_POSIX)
    GpSocketId&             IdRef               (void) noexcept {return iId;}

    inline static void      SClose              (GpSocketId& aId);
    inline static void      SCheckResOrThrow    (int                    aRes,
                                                 std::function<void()>  aFnOnThrow);

private:
    GpSocketId              iId         = GpSocketId_Default();
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
iId        {std::move(aSocket.iId        )},
iIPv       {std::move(aSocket.iIPv       )},
iProtocol  {std::move(aSocket.iProtocol  )},
iAddrLocal {std::move(aSocket.iAddrLocal )},
iAddrRemote{std::move(aSocket.iAddrRemote)},
iFlags     {std::move(aSocket.iFlags     )},
iCloseMode {std::move(aSocket.iCloseMode )}
{
    aSocket.iId         = GpSocketId_Default();
    aSocket.iCloseMode  = CloseModeT::KEEP_ON_DESTRUCT;
}

GpSocket::GpSocket
(
    const ProtocolTE    aProtocol,
    GpSocketFlags       aFlags,
    const CloseModeT    aCloseMode
) noexcept:
iProtocol {aProtocol},
iFlags    {aFlags},
iCloseMode{aCloseMode}
{
}

GpSocket::GpSocket
(
    const GpSocketId    aId,
    const IPvTE         aIpV,
    const ProtocolTE    aProtocol,
    GpSocketFlags       aFlags,
    const CloseModeT    aCloseMode
) noexcept:
iId       {aId},
iIPv      {aIpV},
iProtocol {aProtocol},
iFlags    {aFlags},
iCloseMode{aCloseMode}
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
        reinterpret_cast<char*>(&error),
        &errlen
    );

    THROW_COND_GP
    (
        res == 0,
        [&error]()
        {
            return strerror(error);
        }
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
        iId = GpSocketId_Default();
        return;
    }

    SClose(IdRef());
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

    SCheckResOrThrow(res, {});

    iAddrLocal = GpSocketAddr::SLocalFromSocketId(Id());
    iAddrRemote.Clear();
}

void    GpSocket::Set (GpSocket&& aSocket)
{
    if (this == &aSocket) [[unlikely]]
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

    aSocket.iId         = GpSocketId_Default();
    aSocket.iCloseMode  = CloseModeT::KEEP_ON_DESTRUCT;
}

void    GpSocket::Create (IPvTE aIPv)
{
    if (Id() != GpSocketId_Default()) [[unlikely]]
    {
        Close();
    }

    iIPv = aIPv;

    const ProtocolTE    protocol = Protocol();
    const GpSocketId    socketID = socket
    (
        GpSocketIPv_SSFamily(iIPv),
        int(GpSocketProtocol_Type(protocol)),
        int(GpSocketProtocol_Proto(protocol))
    );

    THROW_COND_GP
    (
        socketID != GpSocketId_Default(),
        []()
        {
            return GpNetworkErrors::SGetLastError();
        }
    );

    iId = socketID;

    ApplyFlags();
}

void    GpSocket::SetFromRaw (const GpSocketId aId)
{
    if (Id() != GpSocketId_Default())
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

#if defined(GP_POSIX)
    if (iFlags.Test(GpSocketFlag::REUSE_PORT))
    {
        SetFlag_ReusePort(true);
    }
#endif// #if defined(GP_POSIX)

    //SetFlag_NoDelay (iFlags.Test(GpSocketFlag::NO_DELAY));
}

void    GpSocket::SetFlag_ReuseAddr (bool aValue)
{
    int option = aValue ? 1 : 0;
    const int res = setsockopt(Id(), SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&option), sizeof(option));

    SCheckResOrThrow(res, {});
}

void    GpSocket::SetFlag_NoBlock (bool aValue)
{
#if defined(GP_POSIX)
    int opts = fcntl(Id(), F_GETFL);

    SCheckResOrThrow(opts, {});

    if (aValue)
    {
        opts = BitOps::Up_by_mask(opts, O_NONBLOCK);
    } else
    {
        opts = BitOps::Down_by_mask(opts, O_NONBLOCK);
    }

    const int res = fcntl(Id(), F_SETFL, opts);

    SCheckResOrThrow(res, {});
#elif defined (GP_OS_WINDOWS)
    unsigned long mode = aValue ? 1 : 0;
    int res = ioctlsocket(Id(), FIONBIO, &mode);

    SCheckResOrThrow(res, {});
#else
#   error Unsupported OS
#endif
}

void    GpSocket::SetFlag_LingerZero (bool aValue)
{
    struct linger sl;
    sl.l_onoff  = aValue ? 1 : 0;
    sl.l_linger = 0;

    const int res = setsockopt(Id(), SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&sl), sizeof(sl));

    SCheckResOrThrow(res, {});
}

void    GpSocket::SetFlag_RecvPktInfo (bool aValue)
{
    int enable  = aValue ? 1 : 0;
    int level   = iIPv == IPvTE::IPv4 ? IPPROTO_IP :IPPROTO_IPV6;

    const int res = setsockopt(Id(), level, IP_PKTINFO, reinterpret_cast<const char*>(&enable), sizeof(enable));

    SCheckResOrThrow(res, {});
}

#if defined(GP_POSIX)
void    GpSocket::SetFlag_ReusePort (bool aValue)
{
    int option      = aValue ? 1 : 0;
    const int res   = setsockopt(Id(), SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<const char*>(&option), sizeof(option));

    SCheckResOrThrow(res, {});
}
#endif// #if defined(GP_POSIX)

void    GpSocket::SClose (GpSocketId& aId)
{
#if defined(GP_POSIX)
    const int res = close(aId);
    aId = GpSocketId_Default();
    SCheckResOrThrow(res, {});
#elif defined(GP_OS_WINDOWS)
    const int res = closesocket(aId);
    aId = GpSocketId_Default();
    SCheckResOrThrow(res, {});
#else
#   error Unsupported OS
#endif
}

void    GpSocket::SCheckResOrThrow
(
    const int               aRes,
    std::function<void()>   aFnOnThrow
)
{
    THROW_COND_GP
    (
        aRes >= 0,
        [&]()
        {
            if (aFnOnThrow)
            {
                aFnOnThrow();
            }

            return GpNetworkErrors::SGetLastError();
        }
    );
}

}// namespace GPlatform

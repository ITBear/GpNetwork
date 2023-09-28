#pragma once

#include "../GpIOObjectId.hpp"
#include "GpSocketProtocol.hpp"
#include "GpSocketFlags.hpp"
#include "GpSocketAddr.hpp"
#include "../../../GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp"
#include "../../../GpCore2/GpUtils/Streams/GpByteWriter.hpp"
#include "../../../GpCore2/GpUtils/Streams/GpByteReaderStorage.hpp"
#include "../../../GpCore2/GpUtils/Streams/GpByteReader.hpp"

namespace GPlatform {

class GpByteWriter;
class GpByteReader;

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

    virtual size_t          Read                (GpByteWriter& aWriter) = 0;
    virtual size_t          Write               (GpByteReader& aReader) = 0;

    inline GpBytesArray     Read                (void);
    inline size_t           Write               (GpSpanPtrByteR aData);

    inline void             CheckForErrors      (void) const;
    inline void             Close               (void);

protected:
    GpSocket&               operator=           (GpSocket&&) noexcept = delete;
    inline void             Set                 (GpSocket&& aSocket);

    inline void             Bind                (const GpSocketAddr& aAddr);
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
    inline void             SetFlag_NoDelay     (bool aValue);

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

GpBytesArray    GpSocket::Read (void)
{
    GpBytesArray dataBuffer;

    GpByteWriterStorageByteArray    dataWriterStorage(dataBuffer);
    GpByteWriter                    dataWriter(dataWriterStorage);

    Read(dataWriter);

    return dataBuffer;
}

size_t  GpSocket::Write (GpSpanPtrByteR aData)
{
    GpByteReaderStorage dataReaderStorage(aData);
    GpByteReader        dataReader(dataReaderStorage);

    return Write(dataReader);
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
        [](){return std::u8string(GpErrno::SGetAndClear());}
    );
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
    SetFlag_ReuseAddr (iFlags.Test(GpSocketFlag::REUSE_ADDR));
    SetFlag_ReusePort (iFlags.Test(GpSocketFlag::REUSE_PORT));
    SetFlag_NoBlock   (iFlags.Test(GpSocketFlag::NO_BLOCK));
    SetFlag_LingerZero(iFlags.Test(GpSocketFlag::LINGER_ZERO));
    SetFlag_NoDelay   (iFlags.Test(GpSocketFlag::NO_DELAY));
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
        [&](){return std::u8string(GpErrno::SGetAndClear());}
    );

    if (aValue) opts = BitOps::Up_by_mask(opts, O_NONBLOCK);
    else        opts = BitOps::Down_by_mask(opts, O_NONBLOCK);

    THROW_COND_GP
    (
        fcntl(Id(), F_SETFL, opts) >= 0,
        [&](){return std::u8string(GpErrno::SGetAndClear());}
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

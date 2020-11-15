#pragma once

#include "GpSocketProtocol.hpp"
#include "GpSocketFlags.hpp"
#include "GpSocketAddr.hpp"

namespace GPlatform {

class GPNETWORK_API GpSocket
{
public:
    CLASS_REMOVE_CTRS(GpSocket)
    CLASS_DECLARE_DEFAULTS(GpSocket)

    using ProtocolT     = GpSocketProtocol;
    using ProtocolTE    = ProtocolT::EnumT;
    using IPvT          = GpSocketIPv;
    using IPvTE         = IPvT::EnumT;

protected:
                            GpSocket            (const ProtocolTE       aProtocol,
                                                 const GpSocketFlags&   aFlags) noexcept;

public:
    virtual                 ~GpSocket           (void) noexcept;

    ProtocolTE              Protocol            (void) const noexcept {return iProtocol;}
    GpSocketAddr::SocketIdT Id                  (void) const noexcept {return iId;}
    IPvTE                   IPv                 (void) const noexcept {return iIPv;}
    const GpSocketFlags&    Flags               (void) const noexcept {return iFlags;}
    const GpSocketAddr&     AddrLocal           (void) const noexcept {return iAddrLocal;}
    const GpSocketAddr&     AddrRemote          (void) const noexcept {return iAddrRemote;}

    virtual size_byte_t     Read                (GpByteWriter& aWriter) = 0;
    virtual size_byte_t     Write               (GpByteReader& aReader) = 0;
    void                    WriteAll            (GpRawPtrByteR aData);

    void                    CheckForErrors      (void) const;

    void                    Close               (void) noexcept;

protected:
    void                    Bind                (const GpSocketAddr& aAddr);
    void                    Create              (IPvTE aIPv);
    GpSocketAddr&           AddrLocal           (void) noexcept {return iAddrLocal;}
    GpSocketAddr&           AddrRemote          (void) noexcept {return iAddrRemote;}

    void                    SetFromRaw          (const GpSocketAddr::SocketIdT aId);

protected:
    void                    ApplyFlags          (void);
    void                    SetFlag_ReuseAddr   (bool aValue);
    void                    SetFlag_ReusePort   (bool aValue);
    void                    SetFlag_NoBlock     (bool aValue);
    void                    SetFlag_LingerZero  (bool aValue);
    void                    SetFlag_NoDelay     (bool aValue);

private:
    GpSocketAddr::SocketIdT iId         = GpSocketAddr::sDefaultSocketId;
    IPvTE                   iIPv        = IPvTE::IPv4;
    const ProtocolTE        iProtocol;
    GpSocketAddr            iAddrLocal;
    GpSocketAddr            iAddrRemote;
    GpSocketFlags           iFlags;
};

}//namespace GPlatform

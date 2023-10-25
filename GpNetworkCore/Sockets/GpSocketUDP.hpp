#pragma once

#include "GpSocket.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketUDP final: public GpSocket
{
public:
    CLASS_REMOVE_CTRS_COPY(GpSocketUDP)
    CLASS_DD(GpSocketUDP)

public:
    inline                          GpSocketUDP     (void) noexcept;
    inline                          GpSocketUDP     (GpSocketUDP&& aSocket) noexcept;
    inline                          GpSocketUDP     (const GpSocketFlags&   aFlags,
                                                     const CloseModeT       aCloseMode) noexcept;
    virtual                         ~GpSocketUDP    (void) noexcept override final;

    inline GpSocketUDP&             operator=       (GpSocketUDP&& aSocket);
    inline void                     Set             (GpSocketUDP&& aSocket);

    static GpSocketUDP::SP          SFromID         (GpIOObjectId       aId,
                                                     const CloseModeT   aCloseMode);

    [[nodiscard]] size_t            Read            (GpByteWriter& aWriter,
                                                     GpSocketAddr& aFromAddrOut);
    [[nodiscard]] size_t            Write           (GpSpanPtrByteR aPacket);
};

GpSocketUDP::GpSocketUDP (void) noexcept:
GpSocket()
{
}

GpSocketUDP::GpSocketUDP (GpSocketUDP&& aSocket) noexcept:
GpSocket(std::move(aSocket))
{
}

GpSocketUDP::GpSocketUDP
(
    const GpSocketFlags&    aFlags,
    const CloseModeT        aCloseMode
) noexcept:
GpSocket(ProtocolT::UDP, aFlags, aCloseMode)
{
}

GpSocketUDP&    GpSocketUDP::operator= (GpSocketUDP&& aSocket)
{
    Set(std::move(aSocket));

    return *this;
}

void    GpSocketUDP::Set (GpSocketUDP&& aSocket)
{
    GpSocket::Set(std::move(aSocket));
}

}//namespace GPlatform

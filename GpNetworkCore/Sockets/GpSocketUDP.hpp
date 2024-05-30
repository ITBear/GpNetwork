#pragma once

#include "GpSocket.hpp"
#include "GpSocketMessageUDP.hpp"

namespace GPlatform {

class GpByteWriter;

class GP_NETWORK_CORE_API GpSocketUDP final: public GpSocket
{
public:
    CLASS_REMOVE_CTRS_COPY(GpSocketUDP)
    CLASS_DD(GpSocketUDP)

public:
    inline                              GpSocketUDP         (void) noexcept;
    inline                              GpSocketUDP         (GpSocketUDP&& aSocket) noexcept;
    inline                              GpSocketUDP         (const GpSocketFlags&   aFlags,
                                                             const CloseModeT       aCloseMode) noexcept;
    virtual                             ~GpSocketUDP        (void) noexcept override final;

    inline GpSocketUDP&                 operator=           (GpSocketUDP&& aSocket);
    inline void                         Set                 (GpSocketUDP&& aSocket);

    void                                Connect             (const GpSocketAddr&    aAddr);

    [[nodiscard]] std::optional<size_t> ReadFrom            (GpByteWriter& aWriter,
                                                             GpSocketAddr& aFromAddrOut);
    [[nodiscard]] std::optional<size_t> Read                (GpByteWriter& aWriter);
    [[nodiscard]] std::optional<size_t> RecvMsg             (GpSocketMessageUDP& aMessageOut);

    [[nodiscard]] bool                  WriteTo             (GpSpanByteR            aData,
                                                             const GpSocketAddr&    aToAddr);
    [[nodiscard]] bool                  Write               (GpSpanByteR aData);
    [[nodiscard]] bool                  SendMsg             (const GpSocketMessageUDP& aMessage);

private:
#if defined(GP_OS_WINDOWS)
    static LPFN_WSARECVMSG              SGetWSARecvMsgPtr   (const GpSocketUDP& aSocket);
    static LPFN_WSASENDMSG              SGetWSASendMsgPtr   (const GpSocketUDP& aSocket);
#endif// #if defined(GP_OS_WINDOWS)

private:
#if defined(GP_OS_WINDOWS)
    LPFN_WSARECVMSG                     iWSARecvMsg = nullptr;// Function pointer for WSARecvMsg
    LPFN_WSASENDMSG                     iWSASendMsg = nullptr;// Function pointer for WSASendMsg
#endif// #if defined(GP_OS_WINDOWS)
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
GpSocket
(
    ProtocolT::UDP,
    aFlags,
    aCloseMode
)
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

}// namespace GPlatform

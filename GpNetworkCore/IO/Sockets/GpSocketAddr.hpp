#pragma once

#include "GpSocketIPv.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketAddr
{
public:
    using IPvT      = GpSocketIPv;
    using IPvTE     = IPvT::EnumT;

#if defined(GP_POSIX)
    using SocketIdT = int;
    static constexpr SocketIdT sDefaultSocketId = -1;
#elif defined(GP_OS_WINDOWS)
    using SocketIdT = SOCKET;
    static constexpr SocketIdT sDefaultSocketId = INVALID_SOCKET;
#endif

public:
                            GpSocketAddr        (void) noexcept;
                            GpSocketAddr        (const GpSocketAddr& aAddr) noexcept;
                            ~GpSocketAddr       (void) noexcept;

    void                    Clear               (void) noexcept;
    void                    Init                (const IPvTE        aIPv,
                                                 std::string_view   aIP,
                                                 const u_int_16     aPort);
    void                    Set                 (const GpSocketAddr& aAddr) noexcept;
    void                    LocalFromSocketId   (SocketIdT aSocketId);
    void                    RemoteFromSocketId  (SocketIdT aSocketId);

    IPvTE                   IPv                 (void) const noexcept {return iIPv;}
    u_int_16                Port                (void) const noexcept;

    const sockaddr*         Raw_sockaddr        (void) const noexcept {return reinterpret_cast<const sockaddr*>(&iAddr);}
    const sockaddr_in*      Raw_sockaddr_v4     (void) const noexcept {return reinterpret_cast<const sockaddr_in*>(&iAddr);}
    const sockaddr_in6*     Raw_sockaddr_v6     (void) const noexcept {return reinterpret_cast<const sockaddr_in6*>(&iAddr);}
    socklen_t               Raw_sockaddrSize    (void) const noexcept {return socklen_t((iIPv == IPvTE::IPv4) ? sizeof(sockaddr_in) :sizeof(sockaddr_in6));}

    std::string             ToString            (void) const;
    std::string             ToStringIP          (void) const;

private:
    sockaddr*               Raw_sockaddr        (void) noexcept {return reinterpret_cast<sockaddr*>(&iAddr);}
    sockaddr_in*            Raw_sockaddr_v4     (void) noexcept {return reinterpret_cast<sockaddr_in*>(&iAddr);}
    sockaddr_in6*           Raw_sockaddr_v6     (void) noexcept {return reinterpret_cast<sockaddr_in6*>(&iAddr);}

public:
    IPvTE                   iIPv    = IPvTE::IPv4;
    sockaddr_storage        iAddr;
};

}//namespace GPlatform

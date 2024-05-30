#pragma once

#include <GpNetwork/GpNetworkCore/Sockets/GpSocketIPv.hpp>
#include <GpCore2/GpUtils/Types/Bits/GpBitOps.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketAddr
{
public:
    CLASS_DD(GpSocketAddr)

    using IPvT  = GpSocketIPv;
    using IPvTE = IPvT::EnumT;

public:
    inline                      GpSocketAddr        (void) noexcept;
    inline                      GpSocketAddr        (const GpSocketAddr& aAddr) noexcept;
    inline                      GpSocketAddr        (GpSocketAddr&& aAddr) noexcept;
    inline                      GpSocketAddr        (IPvTE              aIPv,
                                                     std::string_view   aIP,
                                                     u_int_16           aPort);
    inline                      GpSocketAddr        (std::string_view   aIP,
                                                     u_int_16           aPort);
    inline                      GpSocketAddr        (const sockaddr_in& aSockaddrIn) noexcept;
    inline                      GpSocketAddr        (const sockaddr_in6& aSockaddrIn6) noexcept;
    inline                      ~GpSocketAddr       (void) noexcept;

    inline void                 Clear               (void) noexcept;
    inline bool                 IsEmpty             (void) const noexcept;
    inline void                 SetAutoIPv          (std::string_view   aIP,
                                                     u_int_16           aPort);
    void                        Set                 (IPvTE              aIPv,
                                                     std::string_view   aIP,
                                                     u_int_16           aPort);
    inline void                 Set                 (const GpSocketAddr& aAddr) noexcept;
    inline void                 Set                 (const sockaddr_in& aSockaddrIn) noexcept;
    inline void                 Set                 (const sockaddr_in6& aSockaddrIn6) noexcept;


    inline GpSocketAddr&        operator=           (const GpSocketAddr& aAddr) noexcept;
    inline GpSocketAddr&        operator=           (GpSocketAddr&& aAddr) noexcept;

    inline bool                 operator==          (const GpSocketAddr& aAddr) const noexcept;

    static GpSocketAddr         SLocalFromSocketId  (const GpSocketId aSocketId);
    static GpSocketAddr         SRemoteFromSocketId (const GpSocketId aSocketId);

    inline IPvTE                IPv                 (void) const noexcept;
    inline void                 SetIPv              (IPvTE aIPv) noexcept;
    inline u_int_16             Port                (void) const noexcept;

    inline const sockaddr*      Raw                 (void) const noexcept;
    inline sockaddr*            Raw                 (void) noexcept;
    inline const sockaddr_in*   Raw_v4              (void) const noexcept;
    inline sockaddr_in*         Raw_v4              (void) noexcept;
    inline const sockaddr_in6*  Raw_v6              (void) const noexcept;
    inline sockaddr_in6*        Raw_v6              (void) noexcept;
    inline socklen_t            RawSize             (void) const noexcept;

    std::string                 ToString            (void) const;
    std::string                 ToStringIP          (void) const;

    static IPvTE                SDetectIPv          (std::string_view aIP);

public:
    IPvTE                       iIPv    = IPvTE::IPv4;
    sockaddr_storage            iAddr;
};

GpSocketAddr::GpSocketAddr (void) noexcept
{
    Clear();
}

GpSocketAddr::GpSocketAddr (const GpSocketAddr& aAddr) noexcept:
iIPv{aAddr.iIPv}
{
    Set(aAddr);
}

GpSocketAddr::GpSocketAddr (GpSocketAddr&& aAddr) noexcept:
iIPv{aAddr.iIPv}
{
    Set(aAddr);
}

GpSocketAddr::GpSocketAddr
(
    const IPvTE         aIPv,
    std::string_view    aIP,
    const u_int_16      aPort
)
{
    Set(aIPv, aIP, aPort);
}

GpSocketAddr::GpSocketAddr
(
    std::string_view    aIP,
    const u_int_16      aPort
)
{
    SetAutoIPv(aIP, aPort);
}

GpSocketAddr::GpSocketAddr (const sockaddr_in& aSockaddrIn) noexcept
{
    Set(aSockaddrIn);
}

GpSocketAddr::GpSocketAddr (const sockaddr_in6& aSockaddrIn6) noexcept
{
    Set(aSockaddrIn6);
}

GpSocketAddr::~GpSocketAddr (void) noexcept
{
    Clear();
}

void    GpSocketAddr::Clear (void) noexcept
{
    std::memset(&iAddr, 0, sizeof(decltype(iAddr)));
    iAddr.ss_family = GpSocketIPv_SSFamily(IPvTE::IPv4);
}

bool    GpSocketAddr::IsEmpty (void) const noexcept
{
    constexpr const std::array<u_int_8, sizeof(decltype(iAddr))> zeros = {u_int_8{0}};
    return std::memcmp(&iAddr, zeros.data(), sizeof(decltype(iAddr)));
}

void    GpSocketAddr::SetAutoIPv
(
    std::string_view    aIP,
    const u_int_16      aPort
)
{
    Set
    (
        SDetectIPv(aIP),
        aIP,
        aPort
    );
}

void    GpSocketAddr::Set (const GpSocketAddr& aAddr) noexcept
{
    iIPv = aAddr.iIPv;
    std::memcpy(&iAddr, &aAddr.iAddr, sizeof(decltype(iAddr)));
}

void    GpSocketAddr::Set (const sockaddr_in& aSockaddrIn) noexcept
{
    iIPv = IPvT::IPv4;
    std::memcpy(&iAddr, &aSockaddrIn, sizeof(sockaddr_in));
}

void    GpSocketAddr::Set (const sockaddr_in6& aSockaddrIn6) noexcept
{
    iIPv = IPvT::IPv6;
    std::memcpy(&iAddr, &aSockaddrIn6, sizeof(sockaddr_in6));
}

GpSocketAddr&   GpSocketAddr::operator= (const GpSocketAddr& aAddr) noexcept
{
    Set(aAddr);

    return *this;
}

GpSocketAddr&   GpSocketAddr::operator= (GpSocketAddr&& aAddr) noexcept
{
    Set(aAddr);

    return *this;
}

bool    GpSocketAddr::operator== (const GpSocketAddr& aAddr) const noexcept
{
    if (iIPv != aAddr.iIPv)
    {
        return false;
    }

    if (iIPv == IPvT::IPv4)
    {
        return std::memcmp(&iAddr, &aAddr.iAddr, sizeof(sockaddr_in)) == 0;
    } else
    {
        return std::memcmp(&iAddr, &aAddr.iAddr, sizeof(sockaddr_in6)) == 0;
    }
}

GpSocketAddr::IPvTE GpSocketAddr::IPv (void) const noexcept
{
    return iIPv;
}

void    GpSocketAddr::SetIPv (const IPvTE aIPv) noexcept
{
    iIPv = aIPv;
}

u_int_16    GpSocketAddr::Port (void) const noexcept
{
    u_int_16 port = 0;

    if (IPv() == IPvTE::IPv4)
    {
        port = BitOps::N2H(Raw_v4()->sin_port);
    } else//IPvTE::IPv6
    {
        port = BitOps::N2H(Raw_v6()->sin6_port);
    }

    return port;
}

const sockaddr* GpSocketAddr::Raw (void) const noexcept
{
    return reinterpret_cast<const sockaddr*>(&iAddr);
}

sockaddr*   GpSocketAddr::Raw (void) noexcept
{
    return reinterpret_cast<sockaddr*>(&iAddr);
}

const sockaddr_in*  GpSocketAddr::Raw_v4 (void) const noexcept
{
    return reinterpret_cast<const sockaddr_in*>(&iAddr);
}

sockaddr_in*    GpSocketAddr::Raw_v4 (void) noexcept
{
    return reinterpret_cast<sockaddr_in*>(&iAddr);
}

const sockaddr_in6* GpSocketAddr::Raw_v6 (void) const noexcept
{
    return reinterpret_cast<const sockaddr_in6*>(&iAddr);
}

sockaddr_in6*   GpSocketAddr::Raw_v6 (void) noexcept
{
    return reinterpret_cast<sockaddr_in6*>(&iAddr);
}

socklen_t   GpSocketAddr::RawSize   (void) const noexcept
{
    return socklen_t((iIPv == IPvTE::IPv4)
        ? sizeof(sockaddr_in)
        : sizeof(sockaddr_in6));
}

}// namespace GPlatform

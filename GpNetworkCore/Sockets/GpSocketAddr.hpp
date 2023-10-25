#pragma once

#include "GpSocketIPv.hpp"
#include "../GpIOObjectId.hpp"
#include "../../../GpCore2/GpUtils/Types/Bits/GpBitOps.hpp"
#include "../../../GpCore2/GpUtils/Other/GpErrno.hpp"

namespace GPlatform {

class GpSocketAddr
{
public:
    using IPvT      = GpSocketIPv;
    using IPvTE     = IPvT::EnumT;

public:
    inline                      GpSocketAddr        (void) noexcept;
    inline                      GpSocketAddr        (const GpSocketAddr& aAddr) noexcept;
    inline                      GpSocketAddr        (GpSocketAddr&& aAddr) noexcept;
    inline                      ~GpSocketAddr       (void) noexcept;

    inline void                 Clear               (void) noexcept;
    inline void                 Init                (const IPvTE        aIPv,
                                                     std::u8string_view aIP,
                                                     const u_int_16     aPort);
    inline void                 Set                 (const GpSocketAddr& aAddr) noexcept;
    inline GpSocketAddr&        operator=           (const GpSocketAddr& aAddr) noexcept;
    inline GpSocketAddr&        operator=           (GpSocketAddr&& aAddr) noexcept;

    inline static GpSocketAddr  SLocalFromSocketId  (const GpIOObjectId aSocketId);
    inline static GpSocketAddr  SRemoteFromSocketId (const GpIOObjectId aSocketId);

    inline IPvTE                IPv                 (void) const noexcept;
    inline void                 SetIPv              (const IPvTE aIPv) noexcept;
    inline u_int_16             Port                (void) const noexcept;

    inline const sockaddr*      Raw                 (void) const noexcept;
    inline sockaddr*            Raw                 (void) noexcept;
    inline const sockaddr_in*   Raw_v4              (void) const noexcept;
    inline sockaddr_in*         Raw_v4              (void) noexcept;
    inline const sockaddr_in6*  Raw_v6              (void) const noexcept;
    inline sockaddr_in6*        Raw_v6              (void) noexcept;
    inline socklen_t            RawSize             (void) const noexcept;

    inline std::u8string        ToString            (void) const;
    inline std::u8string        ToStringIP          (void) const;

public:
    IPvTE                       iIPv    = IPvTE::IPv4;
    sockaddr_storage            iAddr;
};

GpSocketAddr::GpSocketAddr (void) noexcept
{
    Clear();
}

GpSocketAddr::GpSocketAddr (const GpSocketAddr& aAddr) noexcept:
iIPv(aAddr.iIPv)
{
    Set(aAddr);
}

GpSocketAddr::GpSocketAddr (GpSocketAddr&& aAddr) noexcept:
iIPv(aAddr.iIPv)
{
    Set(aAddr);
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

void    GpSocketAddr::Init
(
    const IPvTE         aIPv,
    std::u8string_view  aIP,
    const u_int_16      aPort
)
{
    //For windows https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-inetptonw

    const std::u8string ipStr(aIP);
    size_t              maxLength   = 0;
    void*               sinAddrPtr  = nullptr;

    if (aIPv == IPvTE::IPv4)
    {
        maxLength           = INET_ADDRSTRLEN - 1;
        Raw_v4()->sin_port  = BitOps::H2N(aPort);
        sinAddrPtr          = &(Raw_v4()->sin_addr);
    } else//IPvTE::IPv6
    {
        maxLength           = INET6_ADDRSTRLEN - 1;
        Raw_v6()->sin6_port = BitOps::H2N(aPort);
        sinAddrPtr          = &(Raw_v6()->sin6_addr);
    }

    if (   (aIP.length() == 0)
        || (aIP.length() > maxLength)
        || (inet_pton(GpSocketIPv_SSFamily(aIPv), GpUTF::S_UTF8_To_STR(ipStr).data(), sinAddrPtr) != 1))
    {
        THROW_GP(u8"'"_sv + aIP + u8"' is not valid IP address"_sv);
    }
}

void    GpSocketAddr::Set (const GpSocketAddr& aAddr) noexcept
{
    iIPv = aAddr.iIPv;
    std::memcpy(&iAddr, &aAddr.iAddr, sizeof(decltype(iAddr)));
}

GpSocketAddr&   GpSocketAddr::operator= (const GpSocketAddr& aAddr) noexcept
{
    iIPv = aAddr.iIPv;
    std::memcpy(&iAddr, &aAddr.iAddr, sizeof(decltype(iAddr)));

    return *this;
}

GpSocketAddr&   GpSocketAddr::operator= (GpSocketAddr&& aAddr) noexcept
{
    iIPv = aAddr.iIPv;
    std::memcpy(&iAddr, &aAddr.iAddr, sizeof(decltype(iAddr)));

    return *this;
}

GpSocketAddr    GpSocketAddr::SLocalFromSocketId (const GpIOObjectId aSocketId)
{
    GpSocketAddr addr;

    socklen_t sockLen = sizeof(decltype(iAddr));

    const int res = getsockname
    (
        aSocketId,
        addr.Raw(),
        &sockLen
    );

    THROW_COND_GP
    (
        res == 0,
        [](){return std::u8string(GpErrno::SGetAndClear());}
    );

    if (sockLen == sizeof(sockaddr_in))
    {
        addr.iIPv = IPvTE::IPv4;
    } else//sockLen == sizeof(sockaddr_in6)
    {
        addr.iIPv = IPvTE::IPv6;
    }

    return addr;
}

GpSocketAddr    GpSocketAddr::SRemoteFromSocketId (const GpIOObjectId aSocketId)
{
    GpSocketAddr addr;

    socklen_t sockLen = sizeof(decltype(iAddr));

    const int res = getpeername
    (
        aSocketId,
        addr.Raw(),
        &sockLen
    );

    THROW_COND_GP
    (
        res == 0,
        [](){return std::u8string(GpErrno::SGetAndClear());}
    );

    if (sockLen == sizeof(sockaddr_in))
    {
        addr.iIPv = IPvTE::IPv4;
    } else//sockLen == sizeof(sockaddr_in6)
    {
        addr.iIPv = IPvTE::IPv6;
    }

    return addr;
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

std::u8string   GpSocketAddr::ToString (void) const
{
    std::u8string res;
    res.reserve(128);
    res.append(IPvT::SToString(IPv()))
       .append(u8", "_sv)
       .append(ToStringIP())
       .append(u8":"_sv)
       .append(StrOps::SFromUI64(Port()));

    return res;
}

std::u8string   GpSocketAddr::ToStringIP (void) const
{
    const void* addrPtr = nullptr;
    std::array<char, INET6_ADDRSTRLEN>  buff;
    std::memset(buff.data(), 0, buff.size());

    if (IPv() == IPvTE::IPv4)
    {
        addrPtr = &(Raw_v4()->sin_addr);
    } else//IPvTE::IPv6
    {
        addrPtr = &(Raw_v6()->sin6_addr);
    }

    if (inet_ntop(GpSocketIPv_SSFamily(IPv()), addrPtr, buff.data(), buff.size()) == nullptr)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }

    return std::u8string(reinterpret_cast<const char8_t*>(buff.data()));
}

}//namespace GPlatform

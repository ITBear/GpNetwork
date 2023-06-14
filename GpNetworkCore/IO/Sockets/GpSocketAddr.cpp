#include "GpSocketAddr.hpp"

namespace GPlatform {

GpSocketAddr::GpSocketAddr (void) noexcept
{
    Clear();
}

GpSocketAddr::GpSocketAddr (const GpSocketAddr& aAddr) noexcept:
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
        maxLength                   = INET_ADDRSTRLEN - 1;
        Raw_sockaddr_v4()->sin_port = BitOps::H2N(aPort);
        sinAddrPtr                  = &(Raw_sockaddr_v4()->sin_addr);
    } else//IPvTE::IPv6
    {
        maxLength                   = INET6_ADDRSTRLEN - 1;
        Raw_sockaddr_v6()->sin6_port= BitOps::H2N(aPort);
        sinAddrPtr                  = &(Raw_sockaddr_v6()->sin6_addr);
    }

    if ((aIP.length() == 0) ||
        (aIP.length() > maxLength) ||
        (inet_pton(GpSocketIPv_SSFamily(aIPv), GpUTF::S_UTF8_To_STR(ipStr).data(), sinAddrPtr) != 1))
    {
        THROW_GP(u8"'"_sv + aIP + u8"' is not valid IP address"_sv);
    }
}

void    GpSocketAddr::Set (const GpSocketAddr& aAddr) noexcept
{
    iIPv = aAddr.iIPv;
    std::memcpy(&iAddr, &aAddr.iAddr, sizeof(decltype(iAddr)));
}

void    GpSocketAddr::LocalFromSocketId (SocketIdT aSocketId)
{
    Clear();

    socklen_t sockLen = sizeof(decltype(iAddr));

    if (getsockname(aSocketId,
                    Raw_sockaddr(),
                    &sockLen) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }

    if (sockLen == sizeof(sockaddr_in))
    {
        iIPv = IPvTE::IPv4;
    } else//sockLen == sizeof(sockaddr_in6)
    {
        iIPv = IPvTE::IPv6;
    }
}

void    GpSocketAddr::RemoteFromSocketId (SocketIdT /*aSocketId*/)
{
    Clear();

    socklen_t sockLen = sizeof(decltype(iAddr));

    //TODO debug
    /*if (getpeername(aSocketId,
                    Raw_sockaddr(),
                    &sockLen) != 0)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }*/

    if (sockLen == sizeof(sockaddr_in))
    {
        iIPv = IPvTE::IPv4;
    } else//sockLen == sizeof(sockaddr_in6)
    {
        iIPv = IPvTE::IPv6;
    }
}

u_int_16    GpSocketAddr::Port (void) const noexcept
{
    u_int_16 port = 0;

    if (IPv() == IPvTE::IPv4)
    {
        port = BitOps::N2H(Raw_sockaddr_v4()->sin_port);
    } else//IPvTE::IPv6
    {
        port = BitOps::N2H(Raw_sockaddr_v6()->sin6_port);
    }

    return port;
}

std::u8string   GpSocketAddr::ToString (void) const
{
    std::u8string res;
    res.reserve(128);
    res.append(IPvT::SToString(IPv()))
       .append(u8", "_sv)
       .append(ToStringIP())
       .append(u8": "_sv)
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
        addrPtr = &(Raw_sockaddr_v4()->sin_addr);
    } else//IPvTE::IPv6
    {
        addrPtr = &(Raw_sockaddr_v6()->sin6_addr);
    }

    if (inet_ntop(GpSocketIPv_SSFamily(IPv()), addrPtr, buff.data(), buff.size()) == nullptr)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }

    return std::u8string(reinterpret_cast<const char8_t*>(buff.data()));
}

}//namespace GPlatform

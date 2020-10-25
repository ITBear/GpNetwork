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

void    GpSocketAddr::Init (const IPvTE         aIPv,
                            std::string_view    aIP,
                            const u_int_16      aPort)
{
    //For windows https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-inetptonw

    const std::string   ipStr(aIP);
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
        (inet_pton(GpSocketIPv_SSFamily(aIPv), ipStr.data(), sinAddrPtr) != 1))
    {
        THROW_GPE("'"_sv + aIP + "' is not valid IP address"_sv);
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
        THROW_GPE(GpErrno::SGetAndClear());
    }

    if (sockLen == sizeof(sockaddr_in))
    {
        iIPv = IPvTE::IPv4;
    } else//sockLen == sizeof(sockaddr_in6)
    {
        iIPv = IPvTE::IPv6;
    }
}

void    GpSocketAddr::RemoteFromSocketId (SocketIdT aSocketId)
{
    Clear();

    socklen_t sockLen = sizeof(decltype(iAddr));

    if (getpeername(aSocketId,
                    Raw_sockaddr(),
                    &sockLen) != 0)
    {
        THROW_GPE(GpErrno::SGetAndClear());
    }

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

std::string GpSocketAddr::ToString (void) const
{
    std::string res;
    res.reserve(128);
    res.append(IPvT::SToString(IPv()))
       .append(", "_sv)
       .append(ToStringIP())
       .append(": "_sv)
       .append(GpStringOps::SFromUI64(Port()));

    return res;
}

std::string GpSocketAddr::ToStringIP (void) const
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
        THROW_GPE(GpErrno::SGetAndClear());
    }

    /*{
        char myIP[16];
        unsigned int myPort = 1;

        struct sockaddr_in my_addr;
        bzero(&my_addr, sizeof(my_addr));
        inet_ntop(AF_INET, &(Raw_sockaddr_v4()->sin_addr), myIP, sizeof(myIP));
        myPort = ntohs(my_addr.sin_port);

        std::cout << myIP << ", " << myPort << std::endl;
    }*/

    return std::string(buff.data());
}

}//namespace GPlatform

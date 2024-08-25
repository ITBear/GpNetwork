#include <GpCore2/Config/IncludeExt/fmt.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketAddr.hpp>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(conversion)
    GP_WARNING_DISABLE(switch-enum)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#if defined(GP_COMPILER_MSVC)
#   pragma warning(disable : 4365)
#   pragma warning(disable : 5262)
#endif// #if defined(GP_COMPILER_MSVC)

#include <boost/regex.hpp>

GP_WARNING_POP()

namespace GPlatform {

void    GpSocketAddr::Set
(
    const IPvTE         aIPv,
    std::string_view    aIP,
    const u_int_16      aPort
)
{
    // For windows https://docs.microsoft.com/en-us/windows/desktop/api/ws2tcpip/nf-ws2tcpip-inetptonw

    const std::string   ipStr(aIP);
    size_t              maxLength   = 0;
    void*               sinAddrPtr  = nullptr;
    const sa_family_t   sinFamily   = GpSocketIPv_SSFamily(aIPv);

    if (aIPv == IPvTE::IPv4)
    {
        maxLength               = INET_ADDRSTRLEN - 1;
        Raw_v4()->sin_port      = BitOps::H2N(aPort);
        Raw_v4()->sin_family    = NumOps::SConvert<decltype(sockaddr_in::sin_family)>(sinFamily);
        sinAddrPtr              = &(Raw_v4()->sin_addr);
    } else//IPvTE::IPv6
    {
        maxLength               = INET6_ADDRSTRLEN - 1;
        Raw_v6()->sin6_port     = BitOps::H2N(aPort);
        Raw_v6()->sin6_family   = NumOps::SConvert<decltype(sockaddr_in6::sin6_family)>(sinFamily);
        sinAddrPtr              = &(Raw_v6()->sin6_addr);
    }

    if (   (std::size(aIP) == 0)
        || (std::size(aIP) > maxLength)
        || (inet_pton(sinFamily, std::data(ipStr), sinAddrPtr) != 1))
    {
        THROW_GP
        (
            fmt::format
            (
                "'{}' is not valid IP address",
                aIP
            )
        );
    }
}

GpSocketAddr    GpSocketAddr::SLocalFromSocketId (const GpSocketId aSocketId)
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
        []()
        {
            return std::string(GpErrno::SGetAndClear());
        }
    );

    if (sockLen == sizeof(sockaddr_in))
    {
        addr.iIPv = IPvTE::IPv4;
    } else// sockLen == sizeof(sockaddr_in6)
    {
        addr.iIPv = IPvTE::IPv6;
    }

    return addr;
}

GpSocketAddr    GpSocketAddr::SRemoteFromSocketId (const GpSocketId aSocketId)
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
        []()
        {
            return std::string(GpErrno::SGetAndClear());
        }
    );

    if (sockLen == sizeof(sockaddr_in))
    {
        addr.iIPv = IPvTE::IPv4;
    } else// sockLen == sizeof(sockaddr_in6)
    {
        addr.iIPv = IPvTE::IPv6;
    }

    return addr;
}

void    GpSocketAddr::SetPort (u_int_16 aPort) noexcept
{
    if (iIPv == IPvTE::IPv4)
    {
        Raw_v4()->sin_port = BitOps::H2N(aPort);
    } else// IPvTE::IPv6
    {
        Raw_v6()->sin6_port = BitOps::H2N(aPort);
    }
}

std::string GpSocketAddr::ToString (void) const
{
    std::string res;
    res.reserve(128);
    res.append(IPvT::SToString(IPv()))
       .append(", "_sv)
       .append(ToStringIP())
       .append(":"_sv)
       .append(StrOps::SFromUI64(Port()));

    return res;
}

std::string GpSocketAddr::ToStringIP (void) const
{
    const void* addrPtr = nullptr;
    std::array<char, INET6_ADDRSTRLEN>  buff;
    std::memset(std::data(buff), 0, std::size(buff));

    if (IPv() == IPvTE::IPv4)
    {
        addrPtr = &(Raw_v4()->sin_addr);
    } else//IPvTE::IPv6
    {
        addrPtr = &(Raw_v6()->sin6_addr);
    }

    if (inet_ntop(GpSocketIPv_SSFamily(IPv()), addrPtr, std::data(buff), std::size(buff)) == nullptr)
    {
        THROW_GP(GpErrno::SGetAndClear());
    }

    return std::string(reinterpret_cast<const char*>(std::data(buff)));
}

GpSocketAddr::IPvTE GpSocketAddr::SDetectIPv (std::string_view aIP)
{
    const std::string_view ip = aIP;

    // IPv4
    {
        static const boost::regex ipv4Pattern("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");

        if (boost::regex_search(std::begin(ip), std::end(ip), ipv4Pattern))
        {
            return IPvTE::IPv4;
        }
    }

    // IPv6
    {
        static const boost::regex ipv6Pattern("^(?:[A-Fa-f0-9]{1,4}\\:){7}[A-Fa-f0-9]{1,4}$");

        if (boost::regex_search(std::begin(ip), std::end(ip), ipv6Pattern))
        {
            return IPvTE::IPv6;
        }
    }

    THROW_GP
    (
        fmt::format
        (
            "Unable to detect IP version by IP address '{}'",
            aIP
        )
    );
}

}// namespace GPlatform

#include "GpSocketIP.hpp"

#include <GpCore2/GpUtils/Other/GpErrno.hpp>
#include <GpCore2/Config/IncludeExt/fmt.hpp>

namespace GPlatform {

// -------------------------------------- GpSocketIPv4 --------------------------------------------

GpSocketIPv4    GpSocketIPv4::SFromStr (std::string_view aStr)
{
    GpSocketIPv4 ipv4;

    if (aStr.empty())
    {
        return ipv4;
    }

    std::array<char, 24> buffer;
    const size_t sizeToCopy = std::min(std::size(buffer) - 1, std::size(aStr));

    std::memcpy(std::data(buffer), std::data(aStr), sizeToCopy);
    buffer[sizeToCopy] = 0;

    THROW_COND_GP
    (
        inet_pton(AF_INET, std::data(buffer), ipv4.Data().Ptr()) == 1,
        [aStr]()
        {
            return fmt::format("Failed to convert string {} to IPv4. {}", aStr, GpErrno::SGetAndClear());
        }
    );

    return ipv4;
}

std::string GpSocketIPv4::SToStr (const GpSocketIPv4& aIPv4)
{
    std::array<char, INET_ADDRSTRLEN> buffer;

    std::string_view strPtr = inet_ntop(AF_INET, aIPv4.Data().Ptr(), std::data(buffer), INET_ADDRSTRLEN);

    THROW_COND_GP
    (
        !strPtr.empty(),
        []()
        {
            return fmt::format("Failed to convert IPv4 addr to string. {}", GpErrno::SGetAndClear());
        }
    );

    return std::string(strPtr);
}

GpSocketIPv4    GpSocketIPv4::SFromBytes (GpSpanByteR aData)
{
    THROW_COND_GP
    (
        aData.Count() == 4,
        "Datqa size of IPv4 address must be 4 bytes"
    );

    GpSocketIPv4 address;
    address.Data().CopyFrom(aData);

    return address;
}

// -------------------------------------- GpSocketIPv6 --------------------------------------------

GpSocketIPv6    GpSocketIPv6::SFromStr (std::string_view aStr)
{
    GpSocketIPv6 ipv6;

    if (aStr.empty())
    {
        return ipv6;
    }

    std::array<char, 40> buffer;
    const size_t sizeToCopy = std::min(std::size(buffer) - 1, std::size(aStr));

    std::memcpy(std::data(buffer), std::data(aStr), sizeToCopy);
    buffer[sizeToCopy] = 0;

    THROW_COND_GP
    (
        inet_pton(AF_INET6, std::data(buffer), ipv6.Data().Ptr()) == 1,
        [aStr]()
        {
            return fmt::format("Failed to convert string {} to IPv6. {}", aStr, GpErrno::SGetAndClear());
        }
    );

    return ipv6;
}

std::string GpSocketIPv6::SToStr (const GpSocketIPv6& aIPv6)
{
    std::array<char, INET6_ADDRSTRLEN> buffer;

    std::string_view strPtr = inet_ntop(AF_INET6, aIPv6.Data().Ptr(), std::data(buffer), INET6_ADDRSTRLEN);

    THROW_COND_GP
    (
        !strPtr.empty(),
        []()
        {
            return fmt::format("Failed to convert IPv6 addr to string. {}", GpErrno::SGetAndClear());
        }
    );

    return std::string(strPtr);
}

GpSocketIPv6    GpSocketIPv6::SFromBytes (GpSpanByteR aData)
{
    THROW_COND_GP
    (
        aData.Count() == 16,
        "Datqa size of IPv6 address must be16 bytes"
    );

    GpSocketIPv6 address;
    address.Data().CopyFrom(aData);

    return address;
}

}// namespace GPlatform

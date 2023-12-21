#include "GpSocketAddr.hpp"

#include <regex>
#include <fmt/include/fmt/core.h>

namespace GPlatform {

GpSocketAddr::IPvTE GpSocketAddr::SDetectIPv (std::u8string_view aIP)
{
    const std::string_view ip = GpUTF::S_UTF8_To_STR(aIP);

    const std::regex ipv4Pattern("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");
    const std::regex ipv6Pattern("^(?:[A-Fa-f0-9]{1,4}\\:){7}[A-Fa-f0-9]{1,4}$");

    if (std::regex_match(ip.begin(), ip.end(), ipv4Pattern))
    {
        return IPvTE::IPv4;
    } else if (std::regex_match(ip.begin(), ip.end(), ipv6Pattern))
    {
        return IPvTE::IPv6;
    }

    THROW_GP(fmt::format("Unable to detect IP version by IP address '{}'", GpUTF::S_UTF8_To_STR(aIP)));
}

}// namespace GPlatform

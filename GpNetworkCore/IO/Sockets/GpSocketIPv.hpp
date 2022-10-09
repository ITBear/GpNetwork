#pragma once

#include "GpSockets_global.hpp"

namespace GPlatform {

GP_ENUM(GP_NETWORK_CORE_API, GpSocketIPv,
    IPv4,
    IPv6
);

constexpr sa_family_t GpSocketIPv_SSFamily (const GpSocketIPv::EnumT aValue)
{
    constexpr const std::array<sa_family_t, GpSocketIPv::SCount()> s =
    {
        AF_INET,
        AF_INET6
    };

    return s.at(size_t(aValue));
}

}//namespace GPlatform

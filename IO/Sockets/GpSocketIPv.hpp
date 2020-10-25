#pragma once

#include "GpSockets_global.hpp"

namespace GPlatform {

GP_ENUM(GPNETWORK_API, GpSocketIPv,
    IPv4,
    IPv6
);

constexpr sa_family_t GpSocketIPv_SSFamily (const GpSocketIPv::EnumT aValue)
{
    constexpr const GpArray<sa_family_t, GpSocketIPv::SCount().As<size_t>()> s =
    {
        AF_INET,
        AF_INET6
    };

    return s.at(size_t(aValue));
}

}//namespace GPlatform

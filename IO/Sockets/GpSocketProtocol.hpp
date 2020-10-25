#pragma once

#include "GpSockets_global.hpp"

namespace GPlatform {

GP_ENUM(GPNETWORK_API, GpSocketProtocol,
    UDP,
    TCP,
    SCTP
);

constexpr size_t GpSocketProtocol_Proto (const GpSocketProtocol::EnumT aValue)
{
    constexpr const GpArray<size_t, GpSocketProtocol::SCount().As<size_t>()> s =
    {
        IPPROTO_UDP,
        IPPROTO_TCP,
        IPPROTO_SCTP
    };

    return s.at(size_t(aValue));
}

constexpr size_t GpSocketProtocol_Type (const GpSocketProtocol::EnumT aValue)
{
    constexpr const GpArray<size_t, GpSocketProtocol::SCount().As<size_t>()> s =
    {
        SOCK_DGRAM,
        SOCK_STREAM,
        SOCK_STREAM
    };

    return s.at(size_t(aValue));
}

}//namespace GPlatform

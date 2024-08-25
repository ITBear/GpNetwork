#pragma once

#include <GpNetwork/GpNetworkCore/GpNetworkCore_global.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_CORE_API, GpSocketProtocol,
    UDP,
    TCP,
    SCTP
);

constexpr size_t GpSocketProtocol_Proto (const GpSocketProtocol::EnumT aValue)
{
    constexpr const std::array<size_t, GpSocketProtocol::SCount()> s =
    {
        IPPROTO_UDP,
        IPPROTO_TCP,
        IPPROTO_SCTP
    };

    return s.at(size_t(aValue));
}

constexpr size_t GpSocketProtocol_Type (const GpSocketProtocol::EnumT aValue)
{
    constexpr const std::array<size_t, GpSocketProtocol::SCount()> s =
    {
        SOCK_DGRAM,
        SOCK_STREAM,
        SOCK_STREAM
    };

    return s.at(size_t(aValue));
}

}// namespace GPlatform

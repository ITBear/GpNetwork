#pragma once

#include "../GpNetworkCore_global.hpp"

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_CORE_API, GpSocketFlag,
    REUSE_ADDR,
    REUSE_PORT,
    NO_BLOCK,
    LINGER_ZERO,
    RECV_PKT_INFO,

    TCP_NO_DELAY,

    /*SO_KEEPALIVE
    SO_LINGER

    TCP_KEEPIDLE
    TCP_KEEPCNT
    TCP_KEEPINTVL

    TCP_NODELAY
    TCP_CORK
    TCP_QUICKACK

    TCP_MAXSEG*/
);

using GpSocketFlags = GpEnumFlagsST<GpSocketFlag>;

}// namespace GPlatform

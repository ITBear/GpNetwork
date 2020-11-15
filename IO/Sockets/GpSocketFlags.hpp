#pragma once

#include "../../GpNetwork_global.hpp"

namespace GPlatform {

GP_ENUM(GPNETWORK_API, GpSocketFlag,
    REUSE_ADDR,
    REUSE_PORT,
    NO_BLOCK,
    LINGER_ZERO,
    NO_DELAY

    /*SO_KEEPALIVE
    SO_LINGER

    TCP_KEEPIDLE
    TCP_KEEPCNT
    TCP_KEEPINTVL

    TCP_NODELAY
    TCP_CORK
    TCP_QUICKACK*/
);

using GpSocketFlags = GpEnumFlagsST<GpSocketFlag>;

}//namespace GPlatform

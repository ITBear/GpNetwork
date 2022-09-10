#pragma once

#include "../../GpNetworkCore_global.hpp"

namespace GPlatform {

GP_ENUM(GP_NETWORK_CORE_API, GpIOEventType,
    READY_TO_READ,
    READY_TO_WRITE,
    CLOSED,
    ERROR_OCCURRED
);

using GpIOEventsTypes = GpEnumFlagsST<GpIOEventType>;

}//GPlatform

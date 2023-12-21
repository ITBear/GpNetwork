#pragma once

#include "../GpNetworkCore_global.hpp"

#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_CORE_API, GpIOEventType,
    READY_TO_READ,
    READY_TO_WRITE,
    CLOSED,
    ERROR
);

using GpIOEventsTypes = GpEnumFlagsST<GpIOEventType>;

}//GPlatform

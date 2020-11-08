#pragma once

#include "../../GpNetwork_global.hpp"

namespace GPlatform {

GP_ENUM(GPNETWORK_API, GpIOEventType,
    READY_TO_READ,
    READY_TO_WRITE,
    CLOSED,
    ERROR_OCCURRED
);

using GpIOEventsTypes = GpEnumFlagsST<GpIOEventType>;

}//GPlatform

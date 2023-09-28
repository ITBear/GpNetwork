#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../../GpCore2/GpUtils/Types/Enums/GpEnum.hpp"
#include "../../../../GpCore2/GpUtils/Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpBodyPayloadType,
    FIXED_SIZE,
    MULTIPART,
    STREAM
);

}//namespace GPlatform

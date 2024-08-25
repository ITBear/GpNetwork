#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpHeaderType,
    CONTENT_TYPE,
    CONTENT_LENGTH,
    CONNECTION,
    CACHE_CONTROL,
    AUTHORIZATION,
    HOST,
    USER_AGENT
);

std::string_view GP_NETWORK_HTTP_CORE_API GpProtoHeader_EnumToStr(GpHttpHeaderType::EnumT aValue);

}// namespace GPlatform

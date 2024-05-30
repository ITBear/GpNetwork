#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpConnectionFlag,
    CLOSE,
    KEEP_ALIVE
);

std::string_view            GP_NETWORK_HTTP_CORE_API GpProtoHeader_EnumToStr (GpHttpConnectionFlag::EnumT aValue);
GpHttpConnectionFlag::EnumT GP_NETWORK_HTTP_CORE_API GpProtoHeader_StrToEnum (std::string_view aStr, GpHttpConnectionFlag::EnumT);

}// namespace GPlatform

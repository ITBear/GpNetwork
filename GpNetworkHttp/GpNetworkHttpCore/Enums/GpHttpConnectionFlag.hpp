#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../../GpCore2/GpUtils/Types/Enums/GpEnum.hpp"
#include "../../../../GpCore2/GpUtils/Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpConnectionFlag,
    CLOSE,
    KEEP_ALIVE
);

std::u8string_view          GpProtoHeader_EnumToStr (GpHttpConnectionFlag::EnumT aValue);
GpHttpConnectionFlag::EnumT GpProtoHeader_StrToEnum (std::u8string_view aStr, GpHttpConnectionFlag::EnumT);

}//namespace GPlatform

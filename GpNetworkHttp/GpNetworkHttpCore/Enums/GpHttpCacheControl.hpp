
#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpCacheControl,
    NO_STORE
);

std::string_view            GP_NETWORK_HTTP_CORE_API GpProtoHeader_EnumToStr    (GpHttpCacheControl::EnumT aValue);
GpHttpCacheControl::EnumT   GP_NETWORK_HTTP_CORE_API GpProtoHeader_StrToEnum    (std::string_view aStr, GpHttpCacheControl::EnumT);

}// namespace GPlatform

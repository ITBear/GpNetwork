#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpCharset,
    NOT_SET,
    UTF_8
);

std::string_view GP_NETWORK_HTTP_CORE_API GpProtoHeader_EnumToStr (GpHttpCharset::EnumT aValue);

}// namespace GPlatform

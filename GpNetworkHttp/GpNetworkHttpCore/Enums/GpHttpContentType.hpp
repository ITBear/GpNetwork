#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../../GpCore2/GpUtils/Types/Enums/GpEnum.hpp"
#include "../../../../GpCore2/GpUtils/Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpContentType,
    TEXT_PLAIN,
    TEXT_HTML,
    APPLICATION_JSON,
    APPLICATION_XML
);

std::u8string_view GpProtoHeader_EnumToStr (GpHttpContentType::EnumT aValue);

}//namespace GPlatform

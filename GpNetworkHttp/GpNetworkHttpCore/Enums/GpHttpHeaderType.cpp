#include "GpHttpHeaderType.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpHeaderType)

static const std::array<std::string_view, GpHttpHeaderType::SCount()>   _gpHttpHeaderType_sStrValues =
{
    "content-type"_sv,      // CONTENT_TYPE,
    "content-length"_sv,    // CONTENT_LENGTH,
    "connection"_sv,        // CONNECTION
    "cache-control"_sv,     // CACHE_CONTROL
    "authorization"_sv,     // AUTHORIZATION
    "host"_sv               // HOST
};

std::string_view GpProtoHeader_EnumToStr(GpHttpHeaderType::EnumT aValue)
{
    return _gpHttpHeaderType_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

}// namespace GPlatform

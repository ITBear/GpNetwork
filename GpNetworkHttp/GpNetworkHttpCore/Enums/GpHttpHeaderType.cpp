#include "GpHttpHeaderType.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpHeaderType)

static const std::array<std::u8string_view, GpHttpHeaderType::SCount()> _gpHttpHeaderType_sStrValues =
{
    u8"Content-Type"_sv,    //CONTENT_TYPE,
    u8"Content-Length"_sv,  //CONTENT_LENGTH,
    u8"Connection"_sv,      //CONNECTION
    u8"Cache-Control"_sv,   //CACHE_CONTROL
    u8"Authorization"_sv,   //AUTHORIZATION
};

std::u8string_view GpProtoHeader_EnumToStr(GpHttpHeaderType::EnumT aValue)
{
    return _gpHttpHeaderType_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

}//namespace GPlatform

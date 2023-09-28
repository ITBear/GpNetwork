#include "GpHttpContentType.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpContentType)

static const std::array<std::u8string_view, GpHttpContentType::SCount()>    _gpHttpContentType_sStrValues =
{
    u8"text/plain"_sv,      //TEXT_PLAIN
    u8"text/html"_sv,       //TEXT_HTML
    u8"application/json"_sv,//APPLICATION_JSON
    u8"application/xml"_sv  //APPLICATION_XML
};

std::u8string_view GpProtoHeader_EnumToStr (GpHttpContentType::EnumT aValue)
{
    return _gpHttpContentType_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

}//namespace GPlatform

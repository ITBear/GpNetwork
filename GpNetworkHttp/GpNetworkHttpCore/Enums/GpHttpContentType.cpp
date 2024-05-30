#include "GpHttpContentType.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpContentType)

static const std::array<std::string_view, GpHttpContentType::SCount()>  _gpHttpContentType_sStrValues =
{
    "text/plain"_sv,        // TEXT_PLAIN
    "text/html"_sv,         // TEXT_HTML
    "application/json"_sv,  // APPLICATION_JSON
    "application/xml"_sv    // APPLICATION_XML
};

std::string_view GpProtoHeader_EnumToStr (GpHttpContentType::EnumT aValue)
{
    return _gpHttpContentType_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

}// namespace GPlatform

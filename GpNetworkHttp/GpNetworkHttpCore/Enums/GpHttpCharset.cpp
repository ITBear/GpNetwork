#include "GpHttpCharset.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpCharset)

static const std::array<std::string_view, GpHttpCharset::SCount()>  _gpHttpCharset_sStrValues =
{
    ""_sv,              // NOT_SET
    "charset=utf-8"_sv  // UTF_8
};

std::string_view GpProtoHeader_EnumToStr (GpHttpCharset::EnumT aValue)
{
    return _gpHttpCharset_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

}// namespace GPlatform

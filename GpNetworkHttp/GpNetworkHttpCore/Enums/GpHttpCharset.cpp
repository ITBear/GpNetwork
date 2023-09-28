#include "GpHttpCharset.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpCharset)

static const std::array<std::u8string_view, GpHttpCharset::SCount()>    _gpHttpCharset_sStrValues =
{
    u8""_sv,                //NOT_SET
    u8"charset=utf-8"_sv    //UTF_8
};

std::u8string_view GpProtoHeader_EnumToStr (GpHttpCharset::EnumT aValue)
{
    return _gpHttpCharset_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

}//namespace GPlatform

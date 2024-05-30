#include "GpHttpCacheControl.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpCacheControl)

static const std::array<std::string_view, GpHttpCacheControl::SCount()> _gpHttpCacheControl_sStrValues =
{
    "no-store"_sv       // NO_STORE
};

std::string_view GpProtoHeader_EnumToStr (GpHttpCacheControl::EnumT aValue)
{
    return _gpHttpCacheControl_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

GpHttpCacheControl::EnumT   GpProtoHeader_StrToEnum (std::string_view aStr, GpHttpCacheControl::EnumT)
{
    size_t i = 0;

    for (std::string_view v: _gpHttpCacheControl_sStrValues)
    {
        if (StrOps::SIsEqualCaseInsensitive8bit(v, aStr))
        {
            return GpHttpCacheControl::EnumT(GpHttpCacheControl::SID_at(i));
        }

        i++;
    }

    THROW_GP
    (
        "Unknown string value '"_sv + aStr + "' for enum GpHttpCacheControl"_sv
    );
}

}// namespace GPlatform

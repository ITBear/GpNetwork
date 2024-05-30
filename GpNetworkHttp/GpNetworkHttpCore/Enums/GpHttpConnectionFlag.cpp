#include "GpHttpConnectionFlag.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpConnectionFlag)

static const std::array<std::string_view, GpHttpConnectionFlag::SCount()>   _gpHttpConnectionFlag_sStrValues =
{
    "close"_sv,     // CLOSE
    "keep-alive"_sv // KEEP_ALIVE
};

std::string_view GpProtoHeader_EnumToStr (GpHttpConnectionFlag::EnumT aValue)
{
    return _gpHttpConnectionFlag_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

GpHttpConnectionFlag::EnumT GpProtoHeader_StrToEnum (std::string_view aStr, GpHttpConnectionFlag::EnumT)
{
    size_t i = 0;

    for (std::string_view v: _gpHttpConnectionFlag_sStrValues)
    {
        if (StrOps::SIsEqualCaseInsensitive8bit(v, aStr))
        {
            return GpHttpConnectionFlag::EnumT(GpHttpConnectionFlag::SID_at(i));
        }

        i++;
    }

    THROW_GP
    (
        "Unknown string value '"_sv + aStr + "' for enum GpHttpConnectionFlag"_sv
    );
}

}// namespace GPlatform

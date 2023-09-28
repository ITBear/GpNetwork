#include "GpHttpConnectionFlag.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpConnectionFlag)

static const std::array<std::u8string_view, GpHttpConnectionFlag::SCount()> _gpHttpConnectionFlag_sStrValues =
{
    u8"close"_sv,       //CLOSE
    u8"keep-alive"_sv   //KEEP_ALIVE
};

std::u8string_view GpProtoHeader_EnumToStr (GpHttpConnectionFlag::EnumT aValue)
{
    return _gpHttpConnectionFlag_sStrValues.at(NumOps::SConvert<size_t>(GpEnum::value_type(aValue)));
}

GpHttpConnectionFlag::EnumT GpProtoHeader_StrToEnum (std::u8string_view aStr, GpHttpConnectionFlag::EnumT)
{
    size_t i = 0;

    for (std::u8string_view v: _gpHttpConnectionFlag_sStrValues)
    {
        if (StrOps::SIsEqualCaseInsensitive8bit(v, aStr))
        {
            return GpHttpConnectionFlag::EnumT(GpHttpConnectionFlag::SID_at(i));
        }

        i++;
    }

    THROW_GP
    (
        u8"Unknown string value '"_sv + aStr + u8"' for enum GpHttpConnectionFlag"_sv
    );
}

}//namespace GPlatform

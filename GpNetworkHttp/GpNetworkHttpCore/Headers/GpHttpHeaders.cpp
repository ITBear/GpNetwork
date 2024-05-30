#include "GpHttpHeaders.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>
#include <GpCore2/GpUtils/Encoders/GpBase64.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpHttpHeaders, GP_MODULE_UUID)

GpHttpHeaders::~GpHttpHeaders (void) noexcept
{
}

GpHttpHeaders&  GpHttpHeaders::SetAuthBasic
(
    std::string_view aLogin,
    std::string_view aPassword
)
{
    const std::string credentials = aLogin + ":"_sv + aPassword;

    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::AUTHORIZATION,
        "Basic "_sv + GpBase64::SEncodeToStr(GpSpanByteR(std::data(credentials), std::size(credentials)), 0)
    );

    return *this;
}

void    GpHttpHeaders::_SReflectCollectProps (GpReflectProp::SmallVecVal& /*aPropsOut*/)
{
}

void    GpHttpHeaders::SSerialize
(
    const GpHttpHeaders&    aHeaders,
    GpByteWriter&           aWriter
)
{
    const GpProtoHeaderValue::C::MapStr::SP& headers = aHeaders.Headers();

    for (const auto&[name, values]: headers)
    {
        aWriter.Bytes(name);
        aWriter.Bytes(": "_sv);
        aWriter.Bytes(StrOps::SJoin<std::string_view>(values.V().elements, ";"_sv));
        aWriter.Bytes("\r\n"_sv);
    }
}

}// namespace GPlatform

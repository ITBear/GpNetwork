#include "GpHttpHeaders.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpUtils/Encoders/GpBase64.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpHttpHeaders, GP_MODULE_UUID)

GpHttpHeaders::~GpHttpHeaders (void) noexcept
{
}

GpHttpHeaders&  GpHttpHeaders::SetAuthBasic
(
    std::u8string_view aLogin,
    std::u8string_view aPassword
)
{
    const std::u8string credentials = aLogin + u8":"_sv + aPassword;

    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::AUTHORIZATION,
        u8"Basic "_sv + GpBase64::SEncodeToStr(GpSpanPtrByteR(credentials.data(), credentials.size()), 0)
    );

    return *this;
}

void    GpHttpHeaders::_SReflectCollectProps (GpReflectProp::C::Vec::Val& /*aPropsOut*/)
{
}

}//namespace GPlatform

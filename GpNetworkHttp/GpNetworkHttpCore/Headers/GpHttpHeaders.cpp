#include "GpHttpHeaders.hpp"
#include "../../../GpCore2/GpReflection/GpReflectManager.hpp"
#include "../../../GpCore2/GpUtils/Encoders/GpBase64.hpp"

namespace GPlatform {

const std::array<std::u8string, GpHttpHeaderType::SCount()>     GpHttpHeaders::sHeadersNames =
{
    std::u8string(u8"Content-Type"_sv),         //CONTENT_TYPE,
    std::u8string(u8"Content-Length"_sv),       //CONTENT_LENGTH,
    std::u8string(u8"Connection"_sv),           //CONNECTION
    std::u8string(u8"Cache-Control"_sv),        //CACHE_CONTROL
    std::u8string(u8"Authorization"_sv),        //AUTHORIZATION
};

const std::array<std::u8string, GpHttpConnectionFlag::SCount()> GpHttpHeaders::sHttpConnectionFlag =
{
    std::u8string(u8"close"_sv),            //CLOSE
    std::u8string(u8"keep-alive"_sv),       //KEEP_ALIVE
};

const std::array<std::u8string, GpHttpCacheControl::SCount()>   GpHttpHeaders::sHttpCacheControl =
{
    std::u8string(u8"no-store"_sv),         //NO_STORE
};

REFLECT_IMPLEMENT(GpHttpHeaders, GP_MODULE_UUID)

GpHttpHeaders::GpHttpHeaders (void) noexcept:
GpHttpProtoHeaders()
{
}

GpHttpHeaders::GpHttpHeaders (const GpHttpHeaders& aHeaders):
GpHttpProtoHeaders(aHeaders)
{
}

GpHttpHeaders::GpHttpHeaders (GpHttpHeaders&& aHeaders) noexcept:
GpHttpProtoHeaders(std::move(aHeaders))
{
}

GpHttpHeaders::~GpHttpHeaders (void) noexcept
{
}

GpHttpHeaders&  GpHttpHeaders::Replace
(
    const GpHttpHeaderType::EnumT   aType,
    std::u8string_view              aValue
)
{
    GpHttpProtoHeaders::Replace<GpHttpHeaderType, GpHttpHeaders>(aType, aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Replace
(
    const GpHttpHeaderType::EnumT   aType,
    std::u8string&&                 aValue)
{
    GpHttpProtoHeaders::Replace<GpHttpHeaderType, GpHttpHeaders>(aType, std::move(aValue));
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Replace
(
    const GpHttpHeaderType::EnumT   aType,
    const u_int_64                  aValue
)
{
    GpHttpProtoHeaders::Replace<GpHttpHeaderType, GpHttpHeaders>(aType, aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    const GpHttpHeaderType::EnumT   aType,
    std::u8string_view              aValue
)
{
    GpHttpProtoHeaders::Add<GpHttpHeaderType, GpHttpHeaders>(aType, aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    std::u8string_view  aName,
    std::u8string_view  aValue
)
{
    GpHttpProtoHeaders::Add(std::u8string(aName), aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    const GpHttpHeaderType::EnumT   aType,
    std::u8string&&                 aValue
)
{
    GpHttpProtoHeaders::Add<GpHttpHeaderType, GpHttpHeaders>(aType, std::move(aValue));
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    std::u8string_view  aName,
    std::u8string&&     aValue
)
{
    GpHttpProtoHeaders::Add(std::u8string(aName), std::move(aValue));
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    const GpHttpHeaderType::EnumT   aType,
    const u_int_64                  aValue
)
{
    GpHttpProtoHeaders::Add<GpHttpHeaderType, GpHttpHeaders>(aType, aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    std::u8string_view  aName,
    const u_int_64      aValue
)
{
    GpHttpProtoHeaders::Add(std::u8string(aName), aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (const GpHttpContentType::EnumT aContentType)
{
    return Replace(GpHttpHeaderType::CONTENT_TYPE, sContentType.at(size_t(aContentType)));
}

GpHttpHeaders&  GpHttpHeaders::SetContentType
(
    const GpHttpContentType::EnumT  aContentType,
    const GpHttpCharset::EnumT      aCharset
)
{
    return
        Replace(GpHttpHeaderType::CONTENT_TYPE, sContentType.at(size_t(aContentType)))
        .Add(GpHttpHeaderType::CONTENT_TYPE, sCharset.at(size_t(aCharset)));
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (std::u8string aContentType)
{
    return Replace(GpHttpHeaderType::CONTENT_TYPE, std::move(aContentType));
}

GpHttpHeaders&  GpHttpHeaders::SetContentLength (const size_byte_t aLength)
{
    return Replace(GpHttpHeaderType::CONTENT_LENGTH, aLength.As<u_int_64>());
}

GpHttpHeaders&  GpHttpHeaders::SetContentLength (const size_t aLength)
{
    return Replace(GpHttpHeaderType::CONTENT_LENGTH, NumOps::SConvert<u_int_64>(aLength));
}

GpHttpHeaders&  GpHttpHeaders::SetConnection (const GpHttpConnectionFlag::EnumT aConnection)
{
    return Replace(GpHttpHeaderType::CONNECTION, sHttpConnectionFlag.at(size_t(aConnection)));
}

GpHttpHeaders&  GpHttpHeaders::SetCacheControl (const GpHttpCacheControl::EnumT aCacheControl)
{
    return Replace(GpHttpHeaderType::CACHE_CONTROL, sHttpCacheControl.at(size_t(aCacheControl)));
}

GpHttpHeaders&  GpHttpHeaders::SetAuthBasic
(
    std::u8string_view aLogin,
    std::u8string_view aPassword
)
{
    const std::u8string credentials = aLogin + u8":"_sv + aPassword;

    return Replace
    (
        GpHttpHeaderType::AUTHORIZATION,
        u8"Basic "_sv + GpBase64::SEncodeToStr(GpSpanPtrByteR(credentials.data(), credentials.size()), 0)
    );
}

void    GpHttpHeaders::_SReflectCollectProps (GpReflectProp::C::Vec::Val& /*aPropsOut*/)
{
}

}//namespace GPlatform

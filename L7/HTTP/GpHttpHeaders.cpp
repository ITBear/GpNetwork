#include "GpHttpHeaders.hpp"

namespace GPlatform {

const GpArray<std::string, GpHttpHeaderType::SCount().As<size_t>()>     GpHttpHeaders::sHeadersNames =
{
    std::string("Content-Type"_sv),         //CONTENT_TYPE,
    std::string("Content-Length"_sv),       //CONTENT_LENGTH,
    std::string("Connection"_sv),           //CONNECTION
    std::string("Cache-Control"_sv),        //CACHE_CONTROL
    std::string("Authorization"_sv),        //AUTHORIZATION
};

const GpArray<std::string, GpHttpConnectionFlag::SCount().As<size_t>()> GpHttpHeaders::sHttpConnectionFlag =
{
    std::string("close"_sv),            //CLOSE
    std::string("keep-alive"_sv),       //KEEP_ALIVE
};

const GpArray<std::string, GpHttpCacheControl::SCount().As<size_t>()>   GpHttpHeaders::sHttpCacheControl =
{
    std::string("no-store"_sv),         //NO_STORE
};

TYPE_STRUCT_IMPLEMENT(GpHttpHeaders, GP_MODULE_UUID)

GpHttpHeaders::GpHttpHeaders (void) noexcept:
GpProtoHeaders()
{
}

GpHttpHeaders::GpHttpHeaders (const GpHttpHeaders& aHeaders):
GpProtoHeaders(aHeaders)
{
}

GpHttpHeaders::GpHttpHeaders (GpHttpHeaders&& aHeaders) noexcept:
GpProtoHeaders(std::move(aHeaders))
{
}

GpHttpHeaders::~GpHttpHeaders (void) noexcept
{
}

GpHttpHeaders&  GpHttpHeaders::Replace
(
    const GpHttpHeaderType::EnumT   aType,
    std::string_view                aValue
)
{
    GpProtoHeaders::Replace<GpHttpHeaderType, GpHttpHeaders>(aType, aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Replace
(
    const GpHttpHeaderType::EnumT   aType,
    std::string&&                   aValue)
{
    GpProtoHeaders::Replace<GpHttpHeaderType, GpHttpHeaders>(aType, std::move(aValue));
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Replace
(
    const GpHttpHeaderType::EnumT   aType,
    const u_int_64                  aValue
)
{
    GpProtoHeaders::Replace<GpHttpHeaderType, GpHttpHeaders>(aType, aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    const GpHttpHeaderType::EnumT   aType,
    std::string_view                aValue
)
{
    GpProtoHeaders::Add<GpHttpHeaderType, GpHttpHeaders>(aType, aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    std::string_view    aName,
    std::string_view    aValue
)
{
    GpProtoHeaders::Add(std::string(aName), aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    const GpHttpHeaderType::EnumT   aType,
    std::string&&                   aValue
)
{
    GpProtoHeaders::Add<GpHttpHeaderType, GpHttpHeaders>(aType, std::move(aValue));
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    std::string_view    aName,
    std::string&&       aValue
)
{
    GpProtoHeaders::Add(std::string(aName), std::move(aValue));
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    const GpHttpHeaderType::EnumT   aType,
    const u_int_64                  aValue
)
{
    GpProtoHeaders::Add<GpHttpHeaderType, GpHttpHeaders>(aType, aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add
(
    std::string_view    aName,
    const u_int_64      aValue
)
{
    GpProtoHeaders::Add(std::string(aName), aValue);
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (const GpContentType::EnumT aContentType)
{
    return Replace(GpHttpHeaderType::CONTENT_TYPE, sContentType.at(size_t(aContentType)));
}

GpHttpHeaders&  GpHttpHeaders::SetContentType
(
    const GpContentType::EnumT  aContentType,
    const GpCharset::EnumT      aCharset
)
{
    return
        Replace(GpHttpHeaderType::CONTENT_TYPE, sContentType.at(size_t(aContentType)))
        .Add(GpHttpHeaderType::CONTENT_TYPE, sCharset.at(size_t(aCharset)));
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (std::string aContentType)
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
    std::string_view aLogin,
    std::string_view aPassword
)
{
    const std::string credentials = aLogin + ":"_sv + aPassword;

    return Replace(GpHttpHeaderType::AUTHORIZATION, "Basic "_sv + GpBase64::SEncodeToStr(credentials, 0_cnt));
}

void    GpHttpHeaders::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& /*aPropsOut*/)
{   
}

}//namespace GPlatform

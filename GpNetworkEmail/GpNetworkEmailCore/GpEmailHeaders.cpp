#include "GpEmailHeaders.hpp"

namespace GPlatform {

const std::array<std::u8string, GpEmailHeaderType::SCount()>    GpEmailHeaders::sHeadersNames =
{
    std::u8string(u8"Cc"_sv),                           //CC
    std::u8string(u8"Content-Transfer-Encoding"_sv),    //CONTENT_TRANSFER_ENCODING
    std::u8string(u8"Content-Type"_sv),                 //CONTENT_TYPE
    std::u8string(u8"Date"_sv),                         //DATE
    std::u8string(u8"From"_sv),                         //FROM
    std::u8string(u8"List-Unsubscribe"_sv),             //LIST_UNSUBSCRIBE
    std::u8string(u8"Message-ID"_sv),                   //MESSAGE_ID
    std::u8string(u8"MIME-Version"_sv),                 //MIME_VERSION
    std::u8string(u8"Subject"_sv),                      //SUBJECT
    std::u8string(u8"To"_sv)                            //TO
};

REFLECT_IMPLEMENT(GpEmailHeaders, GP_MODULE_UUID)

GpEmailHeaders::GpEmailHeaders (void) noexcept:
GpHttpProtoHeaders()
{
}

GpEmailHeaders::GpEmailHeaders (const GpEmailHeaders& aHeaders):
GpHttpProtoHeaders(aHeaders)
{
}

GpEmailHeaders::GpEmailHeaders (GpEmailHeaders&& aHeaders) noexcept:
GpHttpProtoHeaders(std::move(aHeaders))
{
}

GpEmailHeaders::~GpEmailHeaders (void) noexcept
{
}

GpEmailHeaders& GpEmailHeaders::operator= (const GpEmailHeaders& aHeaders)
{
    Set(aHeaders);
    return *this;
}

GpEmailHeaders& GpEmailHeaders::operator= (GpEmailHeaders&& aHeaders) noexcept
{
    Set(std::move(aHeaders));
    return *this;
}

GpEmailHeaders& GpEmailHeaders::Replace
(
    const GpEmailHeaderType::EnumT  aType,
    std::u8string_view              aValue
)
{
    GpHttpProtoHeaders::Replace<GpEmailHeaderType, GpEmailHeaders>(aType, aValue);
    return *this;
}

GpEmailHeaders& GpEmailHeaders::Replace
(
    const GpEmailHeaderType::EnumT  aType,
    std::u8string&&                 aValue)
{
    GpHttpProtoHeaders::Replace<GpEmailHeaderType, GpEmailHeaders>(aType, std::move(aValue));
    return *this;
}

GpEmailHeaders& GpEmailHeaders::Replace
(
    const GpEmailHeaderType::EnumT  aType,
    const u_int_64                  aValue
)
{
    GpHttpProtoHeaders::Replace<GpEmailHeaderType, GpEmailHeaders>(aType, aValue);
    return *this;
}

GpEmailHeaders& GpEmailHeaders::Add
(
    const GpEmailHeaderType::EnumT  aType,
    std::u8string_view              aValue
)
{
    GpHttpProtoHeaders::Add<GpEmailHeaderType, GpEmailHeaders>(aType, aValue);
    return *this;
}

GpEmailHeaders& GpEmailHeaders::Add
(
    const GpEmailHeaderType::EnumT  aType,
    std::u8string&&                 aValue
)
{
    GpHttpProtoHeaders::Add<GpEmailHeaderType, GpEmailHeaders>(aType, std::move(aValue));
    return *this;
}

GpEmailHeaders& GpEmailHeaders::Add
(
    const GpEmailHeaderType::EnumT  aType,
    const u_int_64                  aValue
)
{
    GpHttpProtoHeaders::Add<GpEmailHeaderType, GpEmailHeaders>(aType, aValue);
    return *this;
}

GpEmailHeaders& GpEmailHeaders::SetContentType (const GpHttpContentType::EnumT aContentType)
{
    return Replace(GpEmailHeaderType::CONTENT_TYPE, sContentType.at(size_t(aContentType)));
}

GpEmailHeaders& GpEmailHeaders::SetContentType
(
    const GpHttpContentType::EnumT  aContentType,
    const GpHttpCharset::EnumT      aCharset
)
{
    return
        Replace(GpEmailHeaderType::CONTENT_TYPE, sContentType.at(size_t(aContentType)))
        .Add(GpEmailHeaderType::CONTENT_TYPE, sCharset.at(size_t(aCharset)));
}

GpEmailHeaders& GpEmailHeaders::SetContentType (std::u8string aContentType)
{
    return Replace(GpEmailHeaderType::CONTENT_TYPE, std::move(aContentType));
}

void    GpEmailHeaders::_SReflectCollectProps (GpReflectProp::C::Vec::Val& /*aPropsOut*/)
{
}

}//namespace GPlatform

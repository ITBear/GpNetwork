#include "GpEmailHeaders.hpp"

namespace GPlatform {

const GpArray<std::string, GpEmailHeaderType::SCount()> GpEmailHeaders::sHeadersNames =
{
    std::string("Cc"_sv),                           //CC
    std::string("Content-Transfer-Encoding"_sv),    //CONTENT_TRANSFER_ENCODING
    std::string("Content-Type"_sv),                 //CONTENT_TYPE
    std::string("Date"_sv),                         //DATE
    std::string("From"_sv),                         //FROM
    std::string("List-Unsubscribe"_sv),             //LIST_UNSUBSCRIBE
    std::string("Message-ID"_sv),                   //MESSAGE_ID
    std::string("MIME-Version"_sv),                 //MIME_VERSION
    std::string("Subject"_sv),                      //SUBJECT
    std::string("To"_sv)                            //TO
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
    std::string_view                aValue
)
{
    GpHttpProtoHeaders::Replace<GpEmailHeaderType, GpEmailHeaders>(aType, aValue);
    return *this;
}

GpEmailHeaders& GpEmailHeaders::Replace
(
    const GpEmailHeaderType::EnumT  aType,
    std::string&&                   aValue)
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
    std::string_view                aValue
)
{
    GpHttpProtoHeaders::Add<GpEmailHeaderType, GpEmailHeaders>(aType, aValue);
    return *this;
}

GpEmailHeaders& GpEmailHeaders::Add
(
    const GpEmailHeaderType::EnumT  aType,
    std::string&&                   aValue
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

GpEmailHeaders& GpEmailHeaders::SetContentType (std::string aContentType)
{
    return Replace(GpEmailHeaderType::CONTENT_TYPE, std::move(aContentType));
}

void    GpEmailHeaders::_SReflectCollectProps (GpReflectProp::C::Vec::Val& /*aPropsOut*/)
{
}

}//namespace GPlatform

#include "GpHttpProtoHeaders.hpp"
#include "../../../GpCore2/GpReflection/GpReflectManager.hpp"

namespace GPlatform {

const std::array<std::u8string, GpHttpContentType::SCount()>    GpHttpProtoHeaders::sContentType =
{
    std::u8string(u8"text/plain"_sv),       //TEXT_PLAIN
    std::u8string(u8"text/html"_sv),        //TEXT_HTML
    std::u8string(u8"application/json"_sv), //APPLICATION_JSON
    std::u8string(u8"application/xml"_sv)   //APPLICATION_XML
};

const std::array<std::u8string, GpHttpCharset::SCount()>        GpHttpProtoHeaders::sCharset =
{
    std::u8string(),                        //NOT_SET
    std::u8string(u8"charset=utf-8"_sv),    //UTF_8
};

REFLECT_IMPLEMENT(GpHttpProtoHeaders, GP_MODULE_UUID)

GpHttpProtoHeaders::GpHttpProtoHeaders (void) noexcept
{
}

GpHttpProtoHeaders::GpHttpProtoHeaders (const GpHttpProtoHeaders& aHeaders):
GpReflectObject(aHeaders),
headers(aHeaders.headers)//TODO: reimplement with deep copy
{
}

GpHttpProtoHeaders::GpHttpProtoHeaders (GpHttpProtoHeaders&& aHeaders) noexcept:
GpReflectObject(std::move(aHeaders)),
headers(std::move(aHeaders.headers))
{
}

GpHttpProtoHeaders::~GpHttpProtoHeaders (void) noexcept
{
}

void    GpHttpProtoHeaders::Set (const GpHttpProtoHeaders& aHeaders)
{
    headers = aHeaders.headers;
}

void    GpHttpProtoHeaders::Set (GpHttpProtoHeaders&& aHeaders) noexcept
{
    headers = std::move(aHeaders.headers);
}

void    GpHttpProtoHeaders::Clear (void)
{
    headers.clear();
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Replace
(
    std::u8string       aName,
    std::u8string_view  aValue
)
{
    return Replace(std::move(aName), std::u8string(aValue));
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Replace
(
    std::u8string   aName,
    std::u8string&& aValue
)
{
    headers[std::move(aName)] = MakeSP<GpProtoHeaderValue>(std::move(aValue));
    return *this;
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Replace
(
    std::u8string   aName,
    const u_int_64  aValue
)
{
    return Replace(std::move(aName), StrOps::SFromUI64(aValue));
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Add
(
    std::u8string       aName,
    std::u8string_view  aValue
)
{
    return Add(std::move(aName), std::u8string(aValue));
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Add
(
    std::u8string   aName,
    std::u8string&& aValue
)
{
    auto iter = headers.find(aName);

    if (iter == headers.end())
    {
        return Replace(std::move(aName), std::move(aValue));
    } else
    {
        iter->second.V().elements.emplace_back(std::move(aValue));
    }

    return *this;
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Add
(
    std::u8string       aName,
    const u_int_64  aValue
)
{
    return Add(std::move(aName), StrOps::SFromUI64(aValue));
}

void    GpHttpProtoHeaders::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(headers);
}

}//namespace GPlatform

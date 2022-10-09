#include "GpHttpProtoHeaders.hpp"

namespace GPlatform {

const std::array<std::string, GpHttpContentType::SCount()>  GpHttpProtoHeaders::sContentType =
{
    std::string("text/plain"_sv),       //TEXT_PLAIN
    std::string("text/html"_sv),        //TEXT_HTML
    std::string("application/json"_sv), //APPLICATION_JSON
    std::string("application/xml"_sv)   //APPLICATION_XML
};

const std::array<std::string, GpHttpCharset::SCount()>      GpHttpProtoHeaders::sCharset =
{
    std::string(""_sv),                 //NOT_SET
    std::string("charset=utf-8"_sv),    //UTF_8
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
    std::string         aName,
    std::string_view    aValue
)
{
    return Replace(std::move(aName), std::string(aValue));
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Replace
(
    std::string     aName,
    std::string&&   aValue
)
{
    headers[std::move(aName)] = MakeSP<GpProtoHeaderValue>(std::move(aValue));
    return *this;
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Replace
(
    std::string     aName,
    const u_int_64  aValue
)
{
    return Replace(std::move(aName), StrOps::SFromUI64(aValue));
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Add
(
    std::string         aName,
    std::string_view    aValue
)
{
    return Add(std::move(aName), std::string(aValue));
}

GpHttpProtoHeaders& GpHttpProtoHeaders::Add
(
    std::string     aName,
    std::string&&   aValue
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
    std::string     aName,
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

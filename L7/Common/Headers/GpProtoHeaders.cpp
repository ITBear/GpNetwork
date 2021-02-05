#include "GpProtoHeaders.hpp"

namespace GPlatform {

const GpArray<std::string, GpContentType::SCount().As<size_t>()>    GpProtoHeaders::sContentType =
{
    std::string("text/plain"_sv),       //TEXT_PLAIN
    std::string("text/html"_sv),        //TEXT_HTML
    std::string("application/json"_sv), //APPLICATION_JSON
    std::string("application/xml"_sv)   //APPLICATION_XML
};

const GpArray<std::string, GpCharset::SCount().As<size_t>()>        GpProtoHeaders::sCharset =
{
    std::string(""_sv),                 //NOT_SET
    std::string("charset=utf-8"_sv),    //UTF_8
};

TYPE_STRUCT_IMPLEMENT(GpProtoHeaders, GP_MODULE_UUID)

GpProtoHeaders::GpProtoHeaders (void) noexcept
{
}

GpProtoHeaders::GpProtoHeaders (const GpProtoHeaders& aHeaders):
headers(aHeaders.headers)//TODO: reimplement with deep copy
{
}

GpProtoHeaders::GpProtoHeaders (GpProtoHeaders&& aHeaders) noexcept:
headers(std::move(aHeaders.headers))
{
}

GpProtoHeaders::~GpProtoHeaders (void) noexcept
{
}

void    GpProtoHeaders::Set (const GpProtoHeaders& aHeaders)
{
    headers = aHeaders.headers;
}

void    GpProtoHeaders::Set (GpProtoHeaders&& aHeaders) noexcept
{
    headers = std::move(aHeaders.headers);
}

void    GpProtoHeaders::Clear (void)
{
    headers.clear();
}

GpProtoHeaders& GpProtoHeaders::Replace
(
    std::string         aName,
    std::string_view    aValue
)
{
    return Replace(std::move(aName), std::string(aValue));
}

GpProtoHeaders& GpProtoHeaders::Replace
(
    std::string     aName,
    std::string&&   aValue
)
{
    headers[std::move(aName)] = MakeSP<GpProtoHeaderValue>(std::move(aValue));
    return *this;
}

GpProtoHeaders& GpProtoHeaders::Replace
(
    std::string     aName,
    const u_int_64  aValue
)
{
    return Replace(std::move(aName), StrOps::SFromUI64(aValue));
}

GpProtoHeaders& GpProtoHeaders::Add
(
    std::string         aName,
    std::string_view    aValue
)
{
    return Add(std::move(aName), std::string(aValue));
}

GpProtoHeaders& GpProtoHeaders::Add
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
        iter->second->elements.emplace_back(std::move(aValue));
    }

    return *this;
}

GpProtoHeaders& GpProtoHeaders::Add
(
    std::string     aName,
    const u_int_64  aValue
)
{
    return Add(std::move(aName), StrOps::SFromUI64(aValue));
}

void    GpProtoHeaders::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(headers);
}

}//namespace GPlatform

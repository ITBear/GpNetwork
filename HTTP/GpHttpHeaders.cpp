#include "GpHttpHeaders.hpp"

namespace GPlatform {

const GpArray<std::string, GpHttpHeaderType::SCount().As<size_t>()>     GpHttpHeaders::sHttpHeaderType =
{
    std::string("Content-Type"_sv),         //CONTENT_TYPE,
    std::string("Content-Length"_sv),       //CONTENT_LENGTH,
    std::string("Connection"_sv),           //CONNECTION
    std::string("Cache-Control"_sv),        //CACHE_CONTROL
};

const GpArray<std::string, GpHttpContentType::SCount().As<size_t>()>    GpHttpHeaders::sHttpContentType =
{
    std::string("text/plain"_sv),       //TEXT_PLAIN
    std::string("application/json"_sv), //APPLICATION_JSON
    std::string("application/xml"_sv)   //APPLICATION_XML
};

const GpArray<std::string, GpHttpCharset::SCount().As<size_t>()>        GpHttpHeaders::sHttpCharset =
{
    std::string(""_sv),                 //NOT_SET
    std::string("charset=utf-8"_sv),    //UTF_8
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

GpHttpHeaders::GpHttpHeaders (void) noexcept
{
}

GpHttpHeaders::GpHttpHeaders (const GpHttpHeaders& aHeaders):
headers(aHeaders.headers)//TODO reimplement with deep copy
{
}

GpHttpHeaders::GpHttpHeaders (GpHttpHeaders&& aHeaders) noexcept:
headers(std::move(aHeaders.headers))
{
}

GpHttpHeaders::~GpHttpHeaders (void) noexcept
{
}

GpHttpHeaders&  GpHttpHeaders::Replace (const GpHttpHeaderType::EnumT   aType,
                                        std::string_view                aValue)
{
    return Replace(aType, std::string(aValue));
}

GpHttpHeaders&  GpHttpHeaders::Replace (const GpHttpHeaderType::EnumT   aType,
                                        std::string&&                   aValue)
{
    const std::string& headerName = GpHttpHeaders::sHttpHeaderType.at(size_t(aType));
    headers[headerName] = MakeSP<GpHttpHeaderValue>(std::move(aValue));

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Replace (const GpHttpHeaderType::EnumT   aType,
                                        const u_int_64                  aValue)
{
    return Replace(aType, GpStringOps::SFromUI64(aValue));
}

GpHttpHeaders&  GpHttpHeaders::Replace (std::string         aName,
                                        std::string_view    aValue)
{
    return Replace(std::move(aName), std::string(aValue));
}

GpHttpHeaders&  GpHttpHeaders::Replace (std::string     aName,
                                        std::string&&   aValue)
{
    headers[std::move(aName)] = MakeSP<GpHttpHeaderValue>(std::move(aValue));
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Replace (std::string     aName,
                                        const u_int_64  aValue)
{
    return Replace(std::move(aName), GpStringOps::SFromUI64(aValue));
}

GpHttpHeaders&  GpHttpHeaders::Add (const GpHttpHeaderType::EnumT   aType,
                                    std::string_view                aValue)
{
    return Add(aType, std::string(aValue));
}

GpHttpHeaders&  GpHttpHeaders::Add (const GpHttpHeaderType::EnumT   aType,
                                    std::string&&                   aValue)
{
    const std::string& headerName = GpHttpHeaders::sHttpHeaderType.at(size_t(aType));
    auto iter = headers.find(headerName);

    if (iter == headers.end())
    {
        return Replace(aType, std::move(aValue));
    } else
    {
        iter->second->elements.emplace_back(std::move(aValue));
    }

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::Add (const GpHttpHeaderType::EnumT   aType,
                                    const u_int_64                  aValue)
{
    return Add(aType, GpStringOps::SFromUI64(aValue));
}

GpHttpHeaders&  GpHttpHeaders::Add (std::string         aName,
                                    std::string_view    aValue)
{
    return Add(std::move(aName), std::string(aValue));
}

GpHttpHeaders&  GpHttpHeaders::Add (std::string     aName,
                                    std::string&&   aValue)
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

GpHttpHeaders&  GpHttpHeaders::Add (std::string     aName,
                                    const u_int_64  aValue)
{
    return Add(std::move(aName), GpStringOps::SFromUI64(aValue));
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (const GpHttpContentType::EnumT aContentType)
{
    return Replace(GpHttpHeaderType::CONTENT_TYPE, sHttpContentType.at(size_t(aContentType)));
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (const GpHttpContentType::EnumT   aContentType,
                                               const GpHttpCharset::EnumT       aCharset)
{
    return
        Replace(GpHttpHeaderType::CONTENT_TYPE, sHttpContentType.at(size_t(aContentType)))
        .Add(GpHttpHeaderType::CONTENT_TYPE, sHttpCharset.at(size_t(aCharset)));
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (std::string aContentType)
{
    return Replace(GpHttpHeaderType::CONTENT_TYPE, std::move(aContentType));
}

GpHttpHeaders&  GpHttpHeaders::SetConnection (const GpHttpConnectionFlag::EnumT aConnection)
{
    return Replace(GpHttpHeaderType::CONNECTION, sHttpConnectionFlag.at(size_t(aConnection)));
}

GpHttpHeaders&  GpHttpHeaders::CacheControl (const GpHttpCacheControl::EnumT aCacheControl)
{
    return Replace(GpHttpHeaderType::CACHE_CONTROL, sHttpCacheControl.at(size_t(aCacheControl)));
}

void    GpHttpHeaders::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(headers);
}

}//namespace GPlatform

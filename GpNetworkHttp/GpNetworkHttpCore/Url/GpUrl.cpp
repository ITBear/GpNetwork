#include "GpUrl.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpUrlPart)

REFLECT_IMPLEMENT(GpUrl, GP_MODULE_UUID)

GpUrl::GpUrl (const GpUrl& aUrl):
GpReflectObject(aUrl),
scheme   (GpReflectUtils::SCopyValue(aUrl.scheme)),
authority(GpReflectUtils::SCopyValue(aUrl.authority)),
path     (GpReflectUtils::SCopyValue(aUrl.path)),
query    (GpReflectUtils::SCopyValue(aUrl.query)),
fragment (GpReflectUtils::SCopyValue(aUrl.fragment))
{
}

GpUrl::GpUrl (GpUrl&& aUrl) noexcept:
scheme   (std::move(aUrl.scheme)),
authority(std::move(aUrl.authority)),
path     (std::move(aUrl.path)),
query    (std::move(aUrl.query)),
fragment (std::move(aUrl.fragment))
{
}

GpUrl::GpUrl
(
    std::string     aScheme,
    GpUrlAuthority  aAuthority,
    std::string     aPath,
    GpUrlQuery      aQuery,
    std::string     aFragment
) noexcept:
scheme   (std::move(aScheme)),
authority(std::move(aAuthority)),
path     (std::move(aPath)),
query    (std::move(aQuery)),
fragment (std::move(aFragment))
{
}

void    GpUrl::Clear (void)
{
    scheme.clear();
    authority.Clear();
    path.clear();
    query.Clear();
    fragment.clear();
}

std::string GpUrl::SchemeHostPortPath (void) const
{
    const std::string port = std::to_string(authority.Port());

    std::string res;
    res.reserve
    (
        scheme.length()
        + 3/*://*/
        + authority.Host().length()
        + 1/*:*/
        + std::size(port)
        + (path.length() > 0 ? (1 + path.length()) : 0)
    );

    res.append(scheme).append("://"_sv).append(authority.Host()).append(":"_sv).append(port);

    if (path.length() > 0)
    {
        res.append("/"_sv).append(path);
    }

    return res;
}

GpUrl::~GpUrl (void) noexcept
{
}

GpUrl&  GpUrl::operator= (const GpUrl& aUrl)
{
    scheme      = aUrl.scheme;
    authority   = aUrl.authority;
    path        = aUrl.path;
    query       = aUrl.query;
    fragment    = aUrl.fragment;

    return *this;
}

GpUrl&  GpUrl::operator= (GpUrl&& aUrl) noexcept
{
    scheme      = std::move(aUrl.scheme);
    authority   = std::move(aUrl.authority);
    path        = std::move(aUrl.path);
    query       = std::move(aUrl.query);
    fragment    = std::move(aUrl.fragment);

    return *this;
}

void    GpUrl::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(scheme);
    PROP(authority);
    PROP(path);
    PROP(query);
    PROP(fragment);
}

std::string GpUrl::SToString (const GpUrl& aUrl)
{
    std::string url;
    url.reserve(512);

    // scheme
    {
        url.append(aUrl.Scheme()).append("://");
    }

    // authority
    {
        url.append(GpUrlAuthority::SToString(aUrl.Authority()));
    }

    // path
    {
        url.append("/").append(aUrl.Path());
    }

    // query
    {
        const GpUrlQuery& query     = aUrl.Query();
        const std::string queryStr  = GpUrlQuery::SToString(query);

        if (!queryStr.empty())
        {
            url.append("?").append(queryStr);
        }
    }

    // fragment
    {
        std::string_view fragment = aUrl.Fragment();
        if (!fragment.empty())
        {
            url.append("#").append(StrOps::SPercentEncode(fragment));
        }
    }

    return url;
}

GpUrl   GpUrl::SFromString (std::string_view aUrl)
{
    // [scheme]://[authority]/[path]?[query][#fragment]
    // scheme - http, https, ftp, ...
    // authority - user_name:password@host:port
    // query - ?param1=value1&param2=value2&...
    // fragment - #value

    // Split scheme and other parts by '://'
    std::string_view schemeStr;
    if (const auto pos = aUrl.find("://"_sv); pos != std::string::npos)
    {
        schemeStr   = aUrl.substr(0, pos);
        aUrl        = aUrl.substr(pos + 3);
    } else
    {
        schemeStr   = "http";
    }

    // Split by '?'
    std::string_view authorityAndPathStr;
    std::string_view queryAndFragmentStr;
    if (const auto pos = aUrl.find("?"_sv); pos != std::string::npos)
    {
        authorityAndPathStr = aUrl.substr(0, pos);
        queryAndFragmentStr = aUrl.substr(pos + 1);
    } else
    {
        authorityAndPathStr = aUrl;
    }

    THROW_COND_GP
    (
        !authorityAndPathStr.empty(),
        "Authority and path parts are empty"
    );

    // Split authorityAndPathStr by '/'
    std::string_view authorityStr;
    std::string_view pathStr;
    if (const auto pos = aUrl.find("/"_sv); pos != std::string::npos)
    {
        authorityStr    = aUrl.substr(0, pos);
        pathStr         = aUrl.substr(pos + 1);
    } else
    {
        authorityStr    = aUrl;
        pathStr         = "/";
    }

    std::string_view queryStr;
    std::string_view fragmentStr;
    if (!queryAndFragmentStr.empty())
    {
        if (const auto pos = queryAndFragmentStr.find("#"_sv); pos != std::string::npos)
        {
            queryStr    = queryAndFragmentStr.substr(0, pos);
            fragmentStr = queryAndFragmentStr.substr(pos + 1);
        } else
        {
            queryStr    = aUrl;
            fragmentStr = "";
        }
    }

    GpUrlQuery      query       = GpUrlQuery::SFromString(queryStr);
    GpUrlAuthority  authority   = GpUrlAuthority::SFromString(authorityStr);

    return GpUrl
    {
        std::string{schemeStr},
        std::move(authority),
        std::string{pathStr},
        std::move(query),
        std::string{fragmentStr}
    };
}

}// namespace GPlatform

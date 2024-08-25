#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Url/GpUrl.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpUrlPartType)

GpUrl::GpUrl (const GpUrl& aUrl):
iScheme   {aUrl.iScheme},
iAuthority{aUrl.iAuthority},
iPath     {aUrl.iPath},
iQuery    {aUrl.iQuery},
iFragment {aUrl.iFragment}
{
}

GpUrl::GpUrl (GpUrl&& aUrl) noexcept:
iScheme   {std::move(aUrl.iScheme)},
iAuthority{std::move(aUrl.iAuthority)},
iPath     {std::move(aUrl.iPath)},
iQuery    {std::move(aUrl.iQuery)},
iFragment {std::move(aUrl.iFragment)}
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
iScheme   {std::move(aScheme)},
iAuthority{std::move(aAuthority)},
iPath     {std::move(aPath)},
iQuery    {std::move(aQuery)},
iFragment {std::move(aFragment)}
{
}

void    GpUrl::Clear (void)
{
    iScheme.clear();
    iAuthority.Clear();
    iPath.clear();
    iQuery.Clear();
    iFragment.clear();
}

GpUrl::~GpUrl (void) noexcept
{
}

std::string GpUrl::ToString (const GpUrlPartTypes& aUrlPartTypes) const
{
    std::string strRes;
    strRes.reserve(512);

    if (aUrlPartTypes.Test(GpUrlPartType::SCHEME))
    {
        strRes.append(iScheme).append("://");
    }

    if (aUrlPartTypes.Test(GpUrlPartType::AUTHORITY_USER_NAME_AND_PASSWORD))
    {
        strRes.append(iAuthority.UserName()).append(":").append(iAuthority.Password());

        if (aUrlPartTypes.Test(GpUrlPartType::AUTHORITY_HOST_AND_PORT))
        {
            strRes.append("@");
        }
    }

    if (aUrlPartTypes.Test(GpUrlPartType::AUTHORITY_HOST_AND_PORT))
    {
        strRes.append(iAuthority.Host());

        const u_int_16 port = iAuthority.Port();
        if (port > 0)
        {
            strRes.append(":").append(std::to_string(port));
        }
    }

    if (aUrlPartTypes.Test(GpUrlPartType::PATH_QUERY_FRAGMENT))
    {
        // path
        strRes.append(iPath);

        // query
        {
            const GpUrlQuery& query     = iQuery;
            const std::string queryStr  = query.ToString();

            if (!queryStr.empty())
            {
                strRes.append("?").append(queryStr);
            }
        }

        // fragment
        {
            std::string_view fragment = iFragment;
            if (!fragment.empty())
            {
                strRes.append("#").append(StrOps::SPercentEncode(fragment));
            }
        }
    }

    return strRes;
}

GpUrl&  GpUrl::operator= (const GpUrl& aUrl)
{
    iScheme     = aUrl.iScheme;
    iAuthority  = aUrl.iAuthority;
    iPath       = aUrl.iPath;
    iQuery      = aUrl.iQuery;
    iFragment   = aUrl.iFragment;

    return *this;
}

GpUrl&  GpUrl::operator= (GpUrl&& aUrl) noexcept
{
    iScheme     = std::move(aUrl.iScheme);
    iAuthority  = std::move(aUrl.iAuthority);
    iPath       = std::move(aUrl.iPath);
    iQuery      = std::move(aUrl.iQuery);
    iFragment   = std::move(aUrl.iFragment);

    return *this;
}

void    GpUrl::SetFromHeaders (const GpHttpHeaders& aHeaders)
{
    iAuthority.SetFromHeaders(aHeaders);
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
    if (const auto pos = authorityAndPathStr.find("/"_sv); pos != std::string::npos)
    {
        authorityStr    = authorityAndPathStr.substr(0, pos);
        pathStr         = authorityAndPathStr.substr(pos);

        if (pathStr.empty()) [[unlikely]]
        {
            pathStr = "/";
        }
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

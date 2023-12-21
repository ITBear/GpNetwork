#include "GpUrl.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpUrlPart)

REFLECT_IMPLEMENT(GpUrl, GP_MODULE_UUID)

GpUrl::~GpUrl (void) noexcept
{
}

void    GpUrl::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(scheme);
    PROP(authority);
    PROP(path);
    PROP(query);
    PROP(fragment);
}

GpUrl   GpUrl::SFromString (std::u8string_view /*aUrl*/)
{
    // scheme:[//authority]/path[?query][#fragment]
    // scheme - http:, https:, ftp:, ...
    // authority - //user_name:password@host:port
    // query - ?param1=value1&param2=value2&...
    // fragment - #value

    GpUrl url;

    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*//Split scheme '://'
    const auto schemeEnd = aUrl.find(u8"://"_sv);
    if (schemeEnd != std::string::npos)
    {
        url.scheme  = aUrl.substr(0, schemeEnd);
        aUrl        = aUrl.substr(schemeEnd, );
    }

    // Split authority and path '/'
    const auto authorityStart = schemeEnd == std::string::npos ? 0 : schemeEnd + 3;
    const auto authorityEnd = aUrl.find(u8'/', authorityStart);
    if (authorityEnd != std::string::npos)
    {
        url.authority   = GpUrlAuthority::SFromString(aUrl.substr(authorityStart, authorityEnd - authorityStart));
        url.path        = aUrl.substr(authorityEnd, url.find(u8'?', authorityEnd) - authorityEnd);
    } else
    {
        url.authority   = GpUrlAuthority::SFromString(aUrl.substr(authorityStart));
    }

    // Split query
    auto queryStart = url.find("?");
    if (queryStart != std::string::npos) {
        auto queryEnd = url.find("#", queryStart);
        auto queryString = url.substr(queryStart + 1, queryEnd - queryStart - 1);

        std::vector<std::string> params;
        boost::split(params, queryString, boost::is_any_of("&"));

        for (const auto& param : params) {
            std::vector<std::string> key_value;
            boost::split(key_value, param, boost::is_any_of("="));
            if (key_value.size() == 2) {
                result.query[key_value[0]] = key_value[1];
            }
        }
    }

    // Split fragment
    auto fragmentStart = url.find("#");
    if (fragmentStart != std::string::npos) {
        result.fragment = url.substr(fragmentStart + 1);
    }*/

    return url;
}


std::u8string   GpUrl::ToString (void) const
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

}//namespace GPlatform

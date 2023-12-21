#pragma once

#include "GpUrlAuthority.hpp"
#include "GpUrlQuery.hpp"

#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpUrlPart,
    SCHEME,
    AUTHORITY_USER_NAME,
    AUTHORITY_PASSWORD,
    AUTHORITY_HOST,
    AUTHORITY_PORT,
    PATH,
    QUERY,
    FRAGMENT
);

class GP_NETWORK_HTTP_CORE_API GpUrl final: public GpReflectObject
{
public:
    CLASS_DD(GpUrl)
    REFLECT_DECLARE(u8"b7b57a7a-444c-4bc2-77f7-be90fa3b5d47"_uuid)

public:
                            GpUrl               (void) noexcept = default;
    inline                  GpUrl               (const GpUrl& aUrl);
    inline                  GpUrl               (GpUrl&& aUrl) noexcept;
    virtual                 ~GpUrl              (void) noexcept override final;

    inline std::u8string    SchemeHostPortPath  (void) const;

    std::u8string_view      Scheme              (void) const noexcept {return scheme;}
    const GpUrlAuthority&   Authority           (void) const noexcept {return authority;}
    std::u8string_view      Path                (void) const noexcept {return path;}
    const GpUrlQuery&       Query               (void) const noexcept {return query;}
    std::u8string_view      Fragment            (void) const noexcept {return fragment;}

    static GpUrl            SFromString         (std::u8string_view aUrl);
    std::u8string           ToString            (void) const;

private:
    // scheme:[//authority]/path[?query][#fragment]
    // scheme - http:, https:, ftp:, ...
    // authority - //user_name:password@host:port
    // query - ?param1=value1&param2=value2&...
    // fragment - #value

    std::u8string           scheme;
    GpUrlAuthority          authority;
    std::u8string           path;
    GpUrlQuery              query;
    std::u8string           fragment;
};

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

std::u8string   GpUrl::SchemeHostPortPath (void) const
{
    const std::u8string port = StrOps::SToString(authority.Port());

    std::u8string res;
    res.reserve
    (
        scheme.length()
        + 3/*://*/
        + authority.Host().length()
        + 1/*:*/
        + port.size()
        + (path.length() > 0 ? (1 + path.length()) : 0)
    );

    res.append(scheme).append(u8"://"_sv).append(authority.Host()).append(u8":"_sv).append(port);

    if (path.length() > 0)
    {
        res.append(u8"/"_sv).append(path);
    }

    return res;
}

}//namespace GPlatform

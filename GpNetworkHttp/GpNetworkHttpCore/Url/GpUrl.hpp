#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Url/GpUrlAuthority.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Url/GpUrlQuery.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpUrlPartType,
    SCHEME,
    AUTHORITY_USER_NAME_AND_PASSWORD,
    AUTHORITY_HOST_AND_PORT,
    PATH_QUERY_FRAGMENT
);

using GpUrlPartTypes = GpEnumFlagsST<GpUrlPartType>;

class GP_NETWORK_HTTP_CORE_API GpUrl
{
public:
    CLASS_DD(GpUrl)

public:
                            GpUrl           (void) noexcept = default;
                            GpUrl           (const GpUrl& aUrl);
                            GpUrl           (GpUrl&& aUrl) noexcept;
                            GpUrl           (std::string    aScheme,
                                             GpUrlAuthority aAuthority,
                                             std::string    aPath,
                                             GpUrlQuery     aQuery,
                                             std::string    aFragment) noexcept;
                            ~GpUrl          (void) noexcept;

    GpUrl&                  operator=       (const GpUrl& aUrl);
    GpUrl&                  operator=       (GpUrl&& aUrl) noexcept;

    void                    Clear           (void);

    std::string             ToString        (const GpUrlPartTypes& aUrlPartTypes) const;

    std::string_view        Scheme          (void) const noexcept {return iScheme;}
    const GpUrlAuthority&   Authority       (void) const noexcept {return iAuthority;}
    GpUrlAuthority&         Authority       (void) noexcept {return iAuthority;}
    std::string_view        Path            (void) const noexcept {return iPath;}
    const GpUrlQuery&       Query           (void) const noexcept {return iQuery;}
    GpUrlQuery&             Query           (void) noexcept {return iQuery;}
    std::string_view        Fragment        (void) const noexcept {return iFragment;}
    void                    SetFromHeaders  (const GpHttpHeaders& aHeaders);

    static GpUrl            SFromString     (std::string_view aUrl);

private:
    // scheme://      authority                   / path ?              query              # fragment
    // scheme://[[user_name:password]@host:[port]]/[path]?[param1=value1&param2=value2&...]#[fragment]

    std::string             iScheme;
    GpUrlAuthority          iAuthority;
    std::string             iPath;
    GpUrlQuery              iQuery;
    std::string             iFragment;
};

}// namespace GPlatform

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
    REFLECT_DECLARE("b7b57a7a-444c-4bc2-77f7-be90fa3b5d47"_uuid)

public:
                            GpUrl               (void) noexcept = default;
                            GpUrl               (const GpUrl& aUrl);
                            GpUrl               (GpUrl&& aUrl) noexcept;
                            GpUrl               (std::string    aScheme,
                                                 GpUrlAuthority aAuthority,
                                                 std::string    aPath,
                                                 GpUrlQuery     aQuery,
                                                 std::string    aFragment) noexcept;
    virtual                 ~GpUrl              (void) noexcept override final;

    GpUrl&                  operator=           (const GpUrl& aUrl);
    GpUrl&                  operator=           (GpUrl&& aUrl) noexcept;

    void                    Clear               (void);

    std::string             SchemeHostPortPath  (void) const;

    std::string_view        Scheme              (void) const noexcept {return scheme;}
    const GpUrlAuthority&   Authority           (void) const noexcept {return authority;}
    GpUrlAuthority&         Authority           (void) noexcept {return authority;}
    std::string_view        Path                (void) const noexcept {return path;}
    const GpUrlQuery&       Query               (void) const noexcept {return query;}
    GpUrlQuery&             Query               (void) noexcept {return query;}
    std::string_view        Fragment            (void) const noexcept {return fragment;}

    static std::string      SToString           (const GpUrl& aUrl);
    static GpUrl            SFromString         (std::string_view aUrl);

private:
    // scheme://      authority                   / path ?              query              # fragment
    // scheme://[[user_name:password]@host:[port]]/[path]?[param1=value1&param2=value2&...]#[fragment]

    std::string             scheme;
    GpUrlAuthority          authority;
    std::string             path;
    GpUrlQuery              query;
    std::string             fragment;
};

}// namespace GPlatform

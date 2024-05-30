#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpUrlQuery final: public GpReflectObject
{
public:
    CLASS_DD(GpUrlQuery)
    REFLECT_DECLARE("a00e65f0-924b-4567-42bd-581da6b103d1"_uuid)

    using ParamsMapT = std::map<std::string, std::string, std::less<>>;

public:
                            GpUrlQuery  (void) noexcept = default;
    inline                  GpUrlQuery  (const GpUrlQuery& aAuthority);
    inline                  GpUrlQuery  (GpUrlQuery&& aAuthority) noexcept;
    inline                  GpUrlQuery  (ParamsMapT aParams) noexcept;
    virtual                 ~GpUrlQuery (void) noexcept override final;

    static  std::string     SToString   (const GpUrlQuery& aQuery);
    static  GpUrlQuery      SFromString (std::string_view aQuery);

    void                    Clear       (void);

    GpUrlQuery&             operator=   (const GpUrlQuery& aUrl);
    GpUrlQuery&             operator=   (GpUrlQuery&& aUrl) noexcept;

private:
    ParamsMapT              params;
};

GpUrlQuery::GpUrlQuery (const GpUrlQuery& aAuthority):
GpReflectObject(aAuthority),
params(GpReflectUtils::SCopyValue(aAuthority.params))
{
}

GpUrlQuery::GpUrlQuery (GpUrlQuery&& aAuthority) noexcept:
GpReflectObject(std::move(aAuthority)),
params(std::move(aAuthority.params))
{
}

GpUrlQuery::GpUrlQuery (ParamsMapT aParams) noexcept:
params(std::move(aParams))
{
}

}// namespace GPlatform

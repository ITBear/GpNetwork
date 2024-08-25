#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCore_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpUrlQuery
{
public:
    CLASS_DD(GpUrlQuery)

    using ParamsMapT = std::map<std::string, std::string, std::less<>>;

public:
                        GpUrlQuery  (void) noexcept = default;
    inline              GpUrlQuery  (const GpUrlQuery& aUrlQuery);
    inline              GpUrlQuery  (GpUrlQuery&& aUrlQuery) noexcept;
    inline              GpUrlQuery  (ParamsMapT aParams) noexcept;
                        ~GpUrlQuery (void) noexcept;

     std::string        ToString    (void) const;
    static  GpUrlQuery  SFromString (std::string_view aUrlQueryStr);

    void                Clear       (void);

    GpUrlQuery&         operator=   (const GpUrlQuery& aUrlQuery);
    GpUrlQuery&         operator=   (GpUrlQuery&& aUrlQuery) noexcept;

private:
    ParamsMapT          iParams;
};

GpUrlQuery::GpUrlQuery (const GpUrlQuery& aUrlQuery):
iParams{aUrlQuery.iParams}
{
}

GpUrlQuery::GpUrlQuery (GpUrlQuery&& aUrlQuery) noexcept:
iParams{std::move(aUrlQuery.iParams)}
{
}

GpUrlQuery::GpUrlQuery (ParamsMapT aParams) noexcept:
iParams{std::move(aParams)}
{
}

}// namespace GPlatform

#pragma once

#include "GpEmailHeaderType.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpEmailHeaders final: public GpHttpProtoHeaders
{
public:
    CLASS_DECLARE_DEFAULTS(GpEmailHeaders)
    REFLECT_DECLARE("debc2d8e-4184-4de4-950f-6641c9275466"_uuid)

public:
                                        GpEmailHeaders      (void) noexcept;
                                        GpEmailHeaders      (const GpEmailHeaders& aHeaders);
                                        GpEmailHeaders      (GpEmailHeaders&& aHeaders) noexcept;
    virtual                             ~GpEmailHeaders     (void) noexcept override final;

    GpEmailHeaders&                     operator=           (const GpEmailHeaders& aHeaders);
    GpEmailHeaders&                     operator=           (GpEmailHeaders&& aHeaders) noexcept;

    GpEmailHeaders&                     Replace             (const GpEmailHeaderType::EnumT aType,
                                                             std::string_view               aValue);
    GpEmailHeaders&                     Replace             (const GpEmailHeaderType::EnumT aType,
                                                             std::string&&                  aValue);
    GpEmailHeaders&                     Replace             (const GpEmailHeaderType::EnumT aType,
                                                             const u_int_64                 aValue);
    GpEmailHeaders&                     Add                 (const GpEmailHeaderType::EnumT aType,
                                                             std::string_view               aValue);
    GpEmailHeaders&                     Add                 (const GpEmailHeaderType::EnumT aType,
                                                             std::string&&                  aValue);
    GpEmailHeaders&                     Add                 (const GpEmailHeaderType::EnumT aType,
                                                             const u_int_64                 aValue);

    GpEmailHeaders&                     SetContentType      (const GpHttpContentType::EnumT aContentType);
    GpEmailHeaders&                     SetContentType      (const GpHttpContentType::EnumT aContentType,
                                                             const GpHttpCharset::EnumT     aCharset);
    GpEmailHeaders&                     SetContentType      (std::string                    aContentType);

public:
    static const GpArray<std::string, GpEmailHeaderType::SCount()>      sHeadersNames;
};

}//namespace GPlatform

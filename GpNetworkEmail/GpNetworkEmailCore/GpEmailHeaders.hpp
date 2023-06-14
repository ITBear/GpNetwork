#pragma once

#include "GpEmailHeaderType.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpEmailHeaders final: public GpHttpProtoHeaders
{
public:
    CLASS_DD(GpEmailHeaders)
    REFLECT_DECLARE(u8"debc2d8e-4184-4de4-950f-6641c9275466"_uuid)

public:
                                GpEmailHeaders      (void) noexcept;
                                GpEmailHeaders      (const GpEmailHeaders& aHeaders);
                                GpEmailHeaders      (GpEmailHeaders&& aHeaders) noexcept;
    virtual                     ~GpEmailHeaders     (void) noexcept override final;

    GpEmailHeaders&             operator=           (const GpEmailHeaders& aHeaders);
    GpEmailHeaders&             operator=           (GpEmailHeaders&& aHeaders) noexcept;

    GpEmailHeaders&             Replace             (const GpEmailHeaderType::EnumT aType,
                                                     std::u8string_view             aValue);
    GpEmailHeaders&             Replace             (const GpEmailHeaderType::EnumT aType,
                                                     std::u8string&&                aValue);
    GpEmailHeaders&             Replace             (const GpEmailHeaderType::EnumT aType,
                                                     const u_int_64                 aValue);
    GpEmailHeaders&             Add                 (const GpEmailHeaderType::EnumT aType,
                                                     std::u8string_view             aValue);
    GpEmailHeaders&             Add                 (const GpEmailHeaderType::EnumT aType,
                                                     std::u8string&&                aValue);
    GpEmailHeaders&             Add                 (const GpEmailHeaderType::EnumT aType,
                                                     const u_int_64                 aValue);

    GpEmailHeaders&             SetContentType      (const GpHttpContentType::EnumT aContentType);
    GpEmailHeaders&             SetContentType      (const GpHttpContentType::EnumT aContentType,
                                                     const GpHttpCharset::EnumT     aCharset);
    GpEmailHeaders&             SetContentType      (std::u8string                  aContentType);

public:
    static const std::array<std::u8string, GpEmailHeaderType::SCount()>     sHeadersNames;
};

}//namespace GPlatform

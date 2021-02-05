#pragma once

#include "GpEmailHeaderType.hpp"
#include "../Common/Headers/GpHeaders.hpp"
#include "../Common/Enums/GpEnums.hpp"

namespace GPlatform {

class GPNETWORK_API GpEmailHeaders final: public GpProtoHeaders
{
public:
    CLASS_DECLARE_DEFAULTS(GpEmailHeaders)
    TYPE_STRUCT_DECLARE("debc2d8e-4184-4de4-950f-6641c9275466"_sv)

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

    GpEmailHeaders&                     SetContentType      (const GpContentType::EnumT aContentType);
    GpEmailHeaders&                     SetContentType      (const GpContentType::EnumT aContentType,
                                                             const GpCharset::EnumT     aCharset);
    GpEmailHeaders&                     SetContentType      (std::string                aContentType);

public:
    static const GpArray<std::string, GpEmailHeaderType::SCount().As<size_t>()>     sHeadersNames;
};

}//namespace GPlatform

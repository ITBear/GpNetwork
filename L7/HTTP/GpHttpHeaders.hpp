#pragma once

#include "GpHttpHeaderType.hpp"
#include "GpHttpConnectionFlag.hpp"
#include "GpHttpCacheControl.hpp"
#include "../Common/Headers/GpHeaders.hpp"
#include "../Common/Enums/GpEnums.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpHeaders final: public GpProtoHeaders
{   
public:
    CLASS_DECLARE_DEFAULTS(GpHttpHeaders)
    TYPE_STRUCT_DECLARE("d5ba25c0-c37b-4568-8410-4478ef6de495"_sv)

public:
                                        GpHttpHeaders       (void) noexcept;
                                        GpHttpHeaders       (const GpHttpHeaders& aHeaders);
                                        GpHttpHeaders       (GpHttpHeaders&& aHeaders) noexcept;
    virtual                             ~GpHttpHeaders      (void) noexcept override final;

    GpHttpHeaders&                      Replace             (const GpHttpHeaderType::EnumT  aType,
                                                             std::string_view               aValue);
    GpHttpHeaders&                      Replace             (const GpHttpHeaderType::EnumT  aType,
                                                             std::string&&                  aValue);
    GpHttpHeaders&                      Replace             (const GpHttpHeaderType::EnumT  aType,
                                                             const u_int_64                 aValue);
    GpHttpHeaders&                      Add                 (const GpHttpHeaderType::EnumT  aType,
                                                             std::string_view               aValue);
    GpHttpHeaders&                      Add                 (const GpHttpHeaderType::EnumT  aType,
                                                             std::string&&                  aValue);
    GpHttpHeaders&                      Add                 (const GpHttpHeaderType::EnumT  aType,
                                                             const u_int_64                 aValue);

    GpHttpHeaders&                      SetContentType      (const GpContentType::EnumT aContentType);
    GpHttpHeaders&                      SetContentType      (const GpContentType::EnumT aContentType,
                                                             const GpCharset::EnumT     aCharset);
    GpHttpHeaders&                      SetContentType      (std::string                aContentType);
    GpHttpHeaders&                      SetContentLength    (const size_byte_t aLength);
    GpHttpHeaders&                      SetContentLength    (const size_t aLength);
    GpHttpHeaders&                      SetConnection       (const GpHttpConnectionFlag::EnumT  aConnection);
    GpHttpHeaders&                      SetCacheControl     (const GpHttpCacheControl::EnumT    aCacheControl);

public:
    static const GpArray<std::string, GpHttpHeaderType::SCount().As<size_t>()>      sHeadersNames;
    static const GpArray<std::string, GpHttpConnectionFlag::SCount().As<size_t>()>  sHttpConnectionFlag;
    static const GpArray<std::string, GpHttpCacheControl::SCount().As<size_t>()>    sHttpCacheControl;
};

}//namespace GPlatform

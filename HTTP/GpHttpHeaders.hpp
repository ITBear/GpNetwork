#pragma once

#include "GpHttpHeaderValue.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpHeaders final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpHttpHeaders)
    TYPE_STRUCT_DECLARE("d5ba25c0-c37b-4568-8410-4478ef6de495"_sv)

public:
                                        GpHttpHeaders       (void) noexcept;
                                        GpHttpHeaders       (const GpHttpHeaders& aHeaders);
                                        GpHttpHeaders       (GpHttpHeaders&& aHeaders) noexcept;
    virtual                             ~GpHttpHeaders      (void) noexcept;

    GpHttpHeaders&                      Replace             (const GpHttpHeaderType::EnumT  aType,
                                                             std::string_view               aValue);
    GpHttpHeaders&                      Replace             (const GpHttpHeaderType::EnumT  aType,
                                                             std::string&&                  aValue);
    GpHttpHeaders&                      Replace             (const GpHttpHeaderType::EnumT  aType,
                                                             const u_int_64                 aValue);
    GpHttpHeaders&                      Replace             (std::string        aName,
                                                             std::string_view   aValue);
    GpHttpHeaders&                      Replace             (std::string        aName,
                                                             std::string&&      aValue);
    GpHttpHeaders&                      Replace             (std::string        aName,
                                                             const u_int_64     aValue);
    GpHttpHeaders&                      Add                 (const GpHttpHeaderType::EnumT  aType,
                                                             std::string_view               aValue);
    GpHttpHeaders&                      Add                 (const GpHttpHeaderType::EnumT  aType,
                                                             std::string&&                  aValue);
    GpHttpHeaders&                      Add                 (const GpHttpHeaderType::EnumT  aType,
                                                             const u_int_64                 aValue);
    GpHttpHeaders&                      Add                 (std::string        aName,
                                                             std::string_view   aValue);
    GpHttpHeaders&                      Add                 (std::string        aName,
                                                             std::string&&      aValue);
    GpHttpHeaders&                      Add                 (std::string        aName,
                                                             const u_int_64     aValue);

    GpHttpHeaders&                      SetContentType      (const GpHttpContentType::EnumT aContentType);
    GpHttpHeaders&                      SetContentType      (const GpHttpContentType::EnumT aContentType,
                                                             const GpHttpCharset::EnumT     aCharset);
    GpHttpHeaders&                      SetContentType      (std::string                    aContentType);
    GpHttpHeaders&                      SetConnection       (const GpHttpConnectionFlag::EnumT  aConnection);
    GpHttpHeaders&                      CacheControl        (const GpHttpCacheControl::EnumT    aCacheControl);

public:
    GpHttpHeaderValue::C::MapStr::SP    headers;

private:
    static const GpArray<std::string, GpHttpHeaderType::SCount().As<size_t>()>      sHttpHeaderType;
    static const GpArray<std::string, GpHttpContentType::SCount().As<size_t>()>     sHttpContentType;
    static const GpArray<std::string, GpHttpCharset::SCount().As<size_t>()>         sHttpCharset;
    static const GpArray<std::string, GpHttpConnectionFlag::SCount().As<size_t>()>  sHttpConnectionFlag;
    static const GpArray<std::string, GpHttpCacheControl::SCount().As<size_t>()>    sHttpCacheControl;
};

}//namespace GPlatform

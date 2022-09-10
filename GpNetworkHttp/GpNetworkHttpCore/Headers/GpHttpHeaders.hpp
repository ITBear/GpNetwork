#pragma once

#include "GpHttpHeaderType.hpp"
#include "GpHttpConnectionFlag.hpp"
#include "GpHttpCacheControl.hpp"
#include "GpHttpVersion.hpp"
#include "GpHttpResponseCode.hpp"
#include "GpHttpRequestType.hpp"
#include "GpHttpProtoHeaders.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpHeaders final: public GpHttpProtoHeaders
{
public:
    CLASS_DECLARE_DEFAULTS(GpHttpHeaders)
    REFLECT_DECLARE("d5ba25c0-c37b-4568-8410-4478ef6de495"_uuid)

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
    GpHttpHeaders&                      Add                 (std::string_view               aName,
                                                             std::string_view               aValue);
    GpHttpHeaders&                      Add                 (const GpHttpHeaderType::EnumT  aType,
                                                             std::string&&                  aValue);
    GpHttpHeaders&                      Add                 (std::string_view               aName,
                                                             std::string&&                  aValue);
    GpHttpHeaders&                      Add                 (const GpHttpHeaderType::EnumT  aType,
                                                             const u_int_64                 aValue);
    GpHttpHeaders&                      Add                 (std::string_view               aName,
                                                             const u_int_64                 aValue);

    GpHttpHeaders&                      SetContentType      (const GpHttpContentType::EnumT aContentType);
    GpHttpHeaders&                      SetContentType      (const GpHttpContentType::EnumT aContentType,
                                                             const GpHttpCharset::EnumT     aCharset);
    GpHttpHeaders&                      SetContentType      (std::string                    aContentType);
    GpHttpHeaders&                      SetContentLength    (const size_byte_t aLength);
    GpHttpHeaders&                      SetContentLength    (const size_t aLength);
    GpHttpHeaders&                      SetConnection       (const GpHttpConnectionFlag::EnumT  aConnection);
    GpHttpHeaders&                      SetCacheControl     (const GpHttpCacheControl::EnumT    aCacheControl);
    GpHttpHeaders&                      SetAuthBasic        (std::string_view aLogin,
                                                             std::string_view aPassword);

public:
    static const GpArray<std::string, GpHttpHeaderType::SCount()>       sHeadersNames;
    static const GpArray<std::string, GpHttpConnectionFlag::SCount()>   sHttpConnectionFlag;
    static const GpArray<std::string, GpHttpCacheControl::SCount()>     sHttpCacheControl;
};

}//namespace GPlatform

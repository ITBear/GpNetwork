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
    CLASS_DD(GpHttpHeaders)
    REFLECT_DECLARE(u8"d5ba25c0-c37b-4568-8410-4478ef6de495"_uuid)

public:
                                GpHttpHeaders       (void) noexcept;
                                GpHttpHeaders       (const GpHttpHeaders& aHeaders);
                                GpHttpHeaders       (GpHttpHeaders&& aHeaders) noexcept;
    virtual                     ~GpHttpHeaders      (void) noexcept override final;

    GpHttpHeaders&              Replace             (const GpHttpHeaderType::EnumT  aType,
                                                     std::u8string_view             aValue);
    GpHttpHeaders&              Replace             (const GpHttpHeaderType::EnumT  aType,
                                                     std::u8string&&                aValue);
    GpHttpHeaders&              Replace             (const GpHttpHeaderType::EnumT  aType,
                                                     const u_int_64                 aValue);
    GpHttpHeaders&              Add                 (const GpHttpHeaderType::EnumT  aType,
                                                     std::u8string_view             aValue);
    GpHttpHeaders&              Add                 (std::u8string_view             aName,
                                                     std::u8string_view             aValue);
    GpHttpHeaders&              Add                 (const GpHttpHeaderType::EnumT  aType,
                                                     std::u8string&&                aValue);
    GpHttpHeaders&              Add                 (std::u8string_view             aName,
                                                     std::u8string&&                aValue);
    GpHttpHeaders&              Add                 (const GpHttpHeaderType::EnumT  aType,
                                                     const u_int_64                 aValue);
    GpHttpHeaders&              Add                 (std::u8string_view             aName,
                                                     const u_int_64                 aValue);

    GpHttpHeaders&              SetContentType      (const GpHttpContentType::EnumT aContentType);
    GpHttpHeaders&              SetContentType      (const GpHttpContentType::EnumT aContentType,
                                                     const GpHttpCharset::EnumT     aCharset);
    GpHttpHeaders&              SetContentType      (std::u8string                  aContentType);
    GpHttpHeaders&              SetContentLength    (const size_byte_t aLength);
    GpHttpHeaders&              SetContentLength    (const size_t aLength);
    GpHttpHeaders&              SetConnection       (const GpHttpConnectionFlag::EnumT  aConnection);
    GpHttpHeaders&              SetCacheControl     (const GpHttpCacheControl::EnumT    aCacheControl);
    GpHttpHeaders&              SetAuthBasic        (std::u8string_view aLogin,
                                                     std::u8string_view aPassword);

public:
    static const std::array<std::u8string, GpHttpHeaderType::SCount()>      sHeadersNames;
    static const std::array<std::u8string, GpHttpConnectionFlag::SCount()>  sHttpConnectionFlag;
    static const std::array<std::u8string, GpHttpCacheControl::SCount()>    sHttpCacheControl;
};

}//namespace GPlatform

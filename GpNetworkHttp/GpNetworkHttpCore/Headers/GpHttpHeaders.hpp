#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpHeaderType.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpConnectionFlag.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpCacheControl.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpContentType.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpCharset.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Headers/GpProtoHeadersMap.hpp>

#include <GpCore2/GpUtils/Types/Units/Other/size_byte_t.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpHeaders final: public GpProtoHeadersMap
{
public:
    CLASS_DD(GpHttpHeaders)

public:
    inline                  GpHttpHeaders       (void) noexcept;
    inline                  GpHttpHeaders       (const GpHttpHeaders& aHeaders);
    inline                  GpHttpHeaders       (GpHttpHeaders&& aHeaders) noexcept;
    virtual                 ~GpHttpHeaders      (void) noexcept override final;

    GpHttpHeaders&          Set                 (GpHttpHeaderType::EnumT    aType,
                                                 std::string                aValue);

    GpHttpHeaders&          SetHost             (std::string                aHost);
    GpHttpHeaders&          SetUserAgent        (std::string                aUserAgent);

    GpHttpHeaders&          SetContentType      (GpHttpContentType::EnumT   aContentType);
    GpHttpHeaders&          SetContentType      (GpHttpContentType::EnumT   aContentType,
                                                 GpHttpCharset::EnumT       aCharset);
    GpHttpHeaders&          SetContentType      (std::string                aContentType);
    GpHttpHeaders&          SetContentType      (std::string                aContentType,
                                                 GpHttpCharset::EnumT       aCharset);

    GpHttpHeaders&          SetContentLength    (size_byte_t aLength);
    GpHttpHeaders&          SetContentLength    (size_t aLength);
    GpHttpHeaders&          SetConnection       (GpHttpConnectionFlag::EnumT    aConnection);
    std::optional<GpHttpConnectionFlag::EnumT>
                            GetConnection       (void) const;

    GpHttpHeaders&          SetCacheControl     (GpHttpCacheControl::EnumT  aCacheControl);
    GpHttpHeaders&          SetAuthBasic        (std::string_view aLogin,
                                                 std::string_view aPassword);

    std::string             ToString            (void) const;

    static void             SSerialize          (const GpHttpHeaders&   aHeaders,
                                                 GpByteWriter&          aWriter);
};

GpHttpHeaders::GpHttpHeaders (void) noexcept:
GpProtoHeadersMap{}
{
}

GpHttpHeaders::GpHttpHeaders (const GpHttpHeaders& aHeaders):
GpProtoHeadersMap{aHeaders}
{
}

GpHttpHeaders::GpHttpHeaders (GpHttpHeaders&& aHeaders) noexcept:
GpProtoHeadersMap{std::move(aHeaders)}
{
}

}// namespace GPlatform

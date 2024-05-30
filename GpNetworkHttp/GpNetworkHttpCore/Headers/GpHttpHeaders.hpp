#pragma once

#include "../Enums/GpHttpHeaderType.hpp"
#include "../Enums/GpHttpConnectionFlag.hpp"
#include "../Enums/GpHttpCacheControl.hpp"
#include "../Enums/GpHttpContentType.hpp"
#include "../Enums/GpHttpCharset.hpp"
#include "GpProtoHeadersMap.hpp"

#include <GpCore2/GpUtils/Types/Units/Other/size_byte_t.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpHeaders final: public GpProtoHeadersMap
{
public:
    CLASS_DD(GpHttpHeaders)
    REFLECT_DECLARE("d5ba25c0-c37b-4568-8410-4478ef6de495"_uuid)

public:
    inline                  GpHttpHeaders       (void) noexcept;
    inline                  GpHttpHeaders       (const GpHttpHeaders& aHeaders);
    inline                  GpHttpHeaders       (GpHttpHeaders&& aHeaders) noexcept;
    virtual                 ~GpHttpHeaders      (void) noexcept override final;

    inline GpHttpHeaders&   Set                 (GpHttpHeaderType::EnumT    aType,
                                                 std::string                aValue);

    inline GpHttpHeaders&   SetContentType      (GpHttpContentType::EnumT   aContentType);
    inline GpHttpHeaders&   SetContentType      (GpHttpContentType::EnumT   aContentType,
                                                 GpHttpCharset::EnumT       aCharset);
    inline GpHttpHeaders&   SetContentType      (std::string                aContentType);
    inline GpHttpHeaders&   SetContentType      (std::string                aContentType,
                                                 GpHttpCharset::EnumT       aCharset);

    inline GpHttpHeaders&   SetContentLength    (size_byte_t aLength);
    inline GpHttpHeaders&   SetContentLength    (size_t aLength);
    inline GpHttpHeaders&   SetConnection       (GpHttpConnectionFlag::EnumT    aConnection);
    inline GpHttpHeaders&   SetCacheControl     (GpHttpCacheControl::EnumT  aCacheControl);
    GpHttpHeaders&          SetAuthBasic        (std::string_view aLogin,
                                                 std::string_view aPassword);

    static void             SSerialize          (const GpHttpHeaders&   aHeaders,
                                                 GpByteWriter&          aWriter);
};

GpHttpHeaders::GpHttpHeaders (void) noexcept:
GpProtoHeadersMap()
{
}

GpHttpHeaders::GpHttpHeaders (const GpHttpHeaders& aHeaders):
GpProtoHeadersMap(aHeaders)
{
}

GpHttpHeaders::GpHttpHeaders (GpHttpHeaders&& aHeaders) noexcept:
GpProtoHeadersMap(std::move(aHeaders))
{
}

GpHttpHeaders&  GpHttpHeaders::Set
(
    const GpHttpHeaderType::EnumT   aType,
    std::string                     aValue
)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType>(aType, std::move(aValue));
    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (const GpHttpContentType::EnumT aContentType)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType, GpHttpContentType>
    (
        GpHttpHeaderType::CONTENT_TYPE,
        aContentType
    );

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetContentType
(
    const GpHttpContentType::EnumT  aContentType,
    const GpHttpCharset::EnumT      aCharset
)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType, GpHttpContentType>
    (
        GpHttpHeaderType::CONTENT_TYPE,
        aContentType
    );

    GpProtoHeadersMap::Add<GpHttpHeaderType, GpHttpCharset>
    (
        GpHttpHeaderType::CONTENT_TYPE,
        aCharset
    );

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetContentType (std::string aContentType)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::CONTENT_TYPE,
        std::move(aContentType)
    );

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetContentType
(
    std::string                 aContentType,
    const GpHttpCharset::EnumT  aCharset
)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::CONTENT_TYPE,
        std::move(aContentType)
    );

    GpProtoHeadersMap::Add<GpHttpHeaderType, GpHttpCharset>
    (
        GpHttpHeaderType::CONTENT_TYPE,
        aCharset
    );

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetContentLength (const size_byte_t aLength)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::CONTENT_LENGTH,
        aLength.As<u_int_64>()
    );

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetContentLength (const size_t aLength)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::CONTENT_LENGTH,
        NumOps::SConvert<u_int_64>(aLength)
    );

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetConnection (const GpHttpConnectionFlag::EnumT aConnection)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType, GpHttpConnectionFlag>
    (
        GpHttpHeaderType::CONNECTION,
        aConnection
    );

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetCacheControl (const GpHttpCacheControl::EnumT aCacheControl)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType, GpHttpCacheControl>
    (
        GpHttpHeaderType::CACHE_CONTROL,
        aCacheControl
    );

    return *this;
}

}// namespace GPlatform

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Headers/GpHttpHeaders.hpp>
#include <GpCore2/GpUtils/Encoders/GpBase64.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>

namespace GPlatform {

GpHttpHeaders::~GpHttpHeaders (void) noexcept
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

GpHttpHeaders&  GpHttpHeaders::SetHost (std::string aHost)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::HOST,
        std::move(aHost)
    );

    return *this;
}

GpHttpHeaders&  GpHttpHeaders::SetUserAgent (std::string aUserAgent)
{
    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::USER_AGENT,
        std::move(aUserAgent)
    );

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

std::optional<GpHttpConnectionFlag::EnumT>  GpHttpHeaders::GetConnection (void) const
{
    return GetValueEnum<GpHttpHeaderType, GpHttpConnectionFlag>(GpHttpHeaderType::CONNECTION, 0);
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

GpHttpHeaders&  GpHttpHeaders::SetAuthBasic
(
    std::string_view aLogin,
    std::string_view aPassword
)
{
    const std::string credentials = aLogin + ":"_sv + aPassword;

    GpProtoHeadersMap::Set<GpHttpHeaderType>
    (
        GpHttpHeaderType::AUTHORIZATION,
        "Basic "_sv + GpBase64::SEncode<std::string>(GpSpanByteR(std::data(credentials), std::size(credentials)), 0)
    );

    return *this;
}

std::string GpHttpHeaders::ToString (void) const
{
    GpBytesArray                    headersData;
    GpByteWriterStorageByteArray    writerStorage(headersData);
    GpByteWriter                    writer(writerStorage);

    GpHttpHeaders::SSerialize(*this, writer);

    return std::string(GpSpanByteR(headersData).AsStringView());
}

void    GpHttpHeaders::SSerialize
(
    const GpHttpHeaders&    aHeaders,
    GpByteWriter&           aWriter
)
{
    const auto& headers = aHeaders.Headers();

    for (const auto&[name, values]: headers)
    {
        aWriter.Bytes(name);
        aWriter.Bytes(": "_sv);
        aWriter.Bytes(StrOps::SJoin<std::string_view>(values.iElements, ";"_sv));
        aWriter.Bytes("\r\n"_sv);
    }
}

}// namespace GPlatform

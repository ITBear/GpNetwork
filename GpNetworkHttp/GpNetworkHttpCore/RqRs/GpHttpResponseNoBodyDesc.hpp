#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Headers/GpHttpHeaders.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpVersion.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpResponseCode.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpResponseNoBodyDesc
{
public:
    CLASS_DD(GpHttpResponseNoBodyDesc)

    using HttpVersionT  = GpHttpVersion;
    using HttpVersionTE = HttpVersionT::EnumT;

    using CodeT         = GpHttpResponseCode;
    using CodeTE        = CodeT::EnumT;

public:
                    GpHttpResponseNoBodyDesc    (void) noexcept = default;
    inline          GpHttpResponseNoBodyDesc    (const GpHttpResponseNoBodyDesc& aResponse);
    inline          GpHttpResponseNoBodyDesc    (GpHttpResponseNoBodyDesc&& aResponse) noexcept;
    inline          GpHttpResponseNoBodyDesc    (CodeTE                 aCode) noexcept;
    inline          GpHttpResponseNoBodyDesc    (CodeTE                 aCode,
                                                 const GpHttpHeaders&   aHeaders);
    inline          GpHttpResponseNoBodyDesc    (CodeTE                 aCode,
                                                 GpHttpHeaders&&        aHeaders) noexcept;
                    ~GpHttpResponseNoBodyDesc   (void) noexcept;

    void            Clear                       (void);
    void            SetHttpVersion              (size_t aMajor,
                                                 size_t aMinor);

public:
    HttpVersionT    http_version    = HttpVersionT::HTTP_1_1;
    CodeT           code            = CodeT::INTERNAL_SERVER_ERROR_500;
    GpHttpHeaders   headers;
};

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc (const GpHttpResponseNoBodyDesc& aResponse):
http_version{aResponse.http_version},
code        {aResponse.code},
headers     {aResponse.headers}
{
}

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc (GpHttpResponseNoBodyDesc&& aResponse) noexcept:
http_version{std::move(aResponse.http_version)},
code        {std::move(aResponse.code)},
headers     {std::move(aResponse.headers)}
{
}

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc
(
    const CodeTE aCode
) noexcept:
code{aCode}
{
}

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc
(
    const CodeTE            aCode,
    const GpHttpHeaders&    aHeaders
):
code   {aCode},
headers{aHeaders}
{
}

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc
(
    const CodeTE    aCode,
    GpHttpHeaders&& aHeaders
) noexcept:
code   {aCode},
headers{std::move(aHeaders)}
{
}

}// namespace GPlatform

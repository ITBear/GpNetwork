#pragma once

#include "../Headers/GpHttpHeaders.hpp"
#include "../Enums/GpHttpVersion.hpp"
#include "../Enums/GpHttpResponseCode.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpResponseNoBodyDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpHttpResponseNoBodyDesc)
    REFLECT_DECLARE(u8"1c48e524-5261-4250-aa72-76d3401cc607"_uuid)

    using HttpVersionT  = GpHttpVersion;
    using HttpVersionTE = HttpVersionT::EnumT;

    using CodeT         = GpHttpResponseCode;
    using CodeTE        = CodeT::EnumT;

public:
                        GpHttpResponseNoBodyDesc    (void) noexcept = default;
    inline              GpHttpResponseNoBodyDesc    (const GpHttpResponseNoBodyDesc& aResponse);
    inline              GpHttpResponseNoBodyDesc    (GpHttpResponseNoBodyDesc&& aResponse) noexcept;
    inline              GpHttpResponseNoBodyDesc    (const CodeTE           aCode) noexcept;
    inline              GpHttpResponseNoBodyDesc    (const CodeTE           aCode,
                                                     const GpHttpHeaders&   aHeaders);
    inline              GpHttpResponseNoBodyDesc    (const CodeTE           aCode,
                                                     GpHttpHeaders&&        aHeaders) noexcept;
    virtual             ~GpHttpResponseNoBodyDesc   (void) noexcept override final;

public:
    HttpVersionT        http_version    = HttpVersionT::HTTP_1_1;
    CodeT               code            = CodeT::INTERNAL_SERVER_ERROR_500;
    GpHttpHeaders       headers;
};

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc (const GpHttpResponseNoBodyDesc& aResponse):
GpReflectObject(aResponse),
http_version(GpReflectUtils::SCopyValue(aResponse.http_version)),
code        (GpReflectUtils::SCopyValue(aResponse.code)),
headers     (GpReflectUtils::SCopyValue(aResponse.headers))
{
}

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc (GpHttpResponseNoBodyDesc&& aResponse) noexcept:
GpReflectObject(std::move(aResponse)),
http_version(std::move(aResponse.http_version)),
code        (std::move(aResponse.code)),
headers     (std::move(aResponse.headers))
{
}

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc
(
    const CodeTE aCode
) noexcept:
code(aCode)
{
}

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc
(
    const CodeTE            aCode,
    const GpHttpHeaders&    aHeaders
):
code   (aCode),
headers(aHeaders)
{
}

GpHttpResponseNoBodyDesc::GpHttpResponseNoBodyDesc
(
    const CodeTE    aCode,
    GpHttpHeaders&& aHeaders
) noexcept:
code   (aCode),
headers(std::move(aHeaders))
{
}

}//namespace GPlatform

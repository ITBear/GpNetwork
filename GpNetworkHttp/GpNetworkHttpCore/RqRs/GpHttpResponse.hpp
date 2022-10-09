#pragma once

#include "../Exceptions/GpHttpException.hpp"
#include "../Headers/GpHttpHeaders.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpResponse final: public GpReflectObject
{
public:
    CLASS_DD(GpHttpResponse)
    REFLECT_DECLARE("1c48e524-5261-4250-aa72-76d3401cc607"_uuid)

    using HttpVersionT      = GpHttpVersion;
    using HttpVersionTE     = HttpVersionT::EnumT;

    using CodeT             = GpHttpResponseCode;
    using CodeTE            = CodeT::EnumT;

public:
                            GpHttpResponse      (void) noexcept = default;
    inline                  GpHttpResponse      (const GpHttpResponse& aResponse);
    inline                  GpHttpResponse      (GpHttpResponse&& aResponse) noexcept;
    inline                  GpHttpResponse      (const CodeTE           aCode) noexcept;
    inline                  GpHttpResponse      (const CodeTE           aCode,
                                                 const GpHttpHeaders&   aHeaders);
    inline                  GpHttpResponse      (const CodeTE           aCode,
                                                 GpHttpHeaders&&        aHeaders) noexcept;
    inline                  GpHttpResponse      (const CodeTE           aCode,
                                                 const GpHttpHeaders&   aHeaders,
                                                 GpBytesArray&&         aBody);
    inline                  GpHttpResponse      (const CodeTE           aCode,
                                                 GpHttpHeaders&&        aHeaders,
                                                 GpBytesArray&&         aBody) noexcept;
                            ~GpHttpResponse     (void) noexcept;

    void                    SetFromException    (const GpHttpException& aHttpEx);
    void                    SetFromException    (const GpException& aEx);
    void                    SetFromException    (const std::exception& aEx);

    inline std::string      ToString            (void) const;

public:
    HttpVersionT            http_version    = HttpVersionT::HTTP_1_1;
    CodeT                   code            = CodeT::INTERNAL_SERVER_ERROR_500;
    GpHttpHeaders           headers;
    GpBytesArray            body;
};

GpHttpResponse::GpHttpResponse (const GpHttpResponse& aResponse):
GpReflectObject(aResponse),
http_version(aResponse.http_version),
code(aResponse.code),
headers(aResponse.headers),
body(aResponse.body)
{
}

GpHttpResponse::GpHttpResponse (GpHttpResponse&& aResponse) noexcept:
GpReflectObject(std::move(aResponse)),
http_version(std::move(aResponse.http_version)),
code(std::move(aResponse.code)),
headers(std::move(aResponse.headers)),
body(std::move(aResponse.body))
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE aCode
) noexcept:
code(aCode)
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE            aCode,
    const GpHttpHeaders&    aHeaders
):
code(aCode),
headers(aHeaders)
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE        aCode,
    GpHttpHeaders&&     aHeaders
) noexcept:
code(aCode),
headers(std::move(aHeaders))
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE            aCode,
    const GpHttpHeaders&    aHeaders,
    GpBytesArray&&          aBody
):
code(aCode),
headers(aHeaders),
body(std::move(aBody))
{
}

GpHttpResponse::GpHttpResponse
(
    const CodeTE    aCode,
    GpHttpHeaders&& aHeaders,
    GpBytesArray&&  aBody
) noexcept:
code(aCode),
headers(std::move(aHeaders)),
body(std::move(aBody))
{
}

std::string GpHttpResponse::ToString (void) const
{
    //TODO: implement
    return "\n\n\n[GpHttpRequest::ToString]: NOT IMPLEMENTED";
}

}//namespace GPlatform

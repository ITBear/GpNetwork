#pragma once

#include "../Headers/GpHttpHeaders.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequest final: public GpReflectObject
{
public:
    CLASS_DECLARE_DEFAULTS(GpHttpRequest)
    REFLECT_DECLARE("4b00f15a-7ce1-415f-be93-cba741adbe09"_uuid)

    using HttpVersionT      = GpHttpVersion;
    using HttpVersionTE     = HttpVersionT::EnumT;

    using HttpRequestTypeT  = GpHttpRequestType;
    using HttpRequestTypeTE = HttpRequestTypeT::EnumT;

public:
    inline                  GpHttpRequest   (void) noexcept = default;
    inline                  GpHttpRequest   (const GpHttpRequest& aRequest);
    inline                  GpHttpRequest   (GpHttpRequest&& aRequest) noexcept;
    inline                  GpHttpRequest   (const HttpVersionTE        aHttpVersion,
                                             const HttpRequestTypeTE    aRequestType,
                                             std::string_view           aUrl,
                                             const GpHttpHeaders&       aHeaders,
                                             GpBytesArray&&             aBody);
                            ~GpHttpRequest  (void) noexcept;

    void                    SetHttpVersion  (const size_t aMajor, const size_t aMinor);

    inline std::string      ToString        (void) const;

public:
    HttpVersionT            http_version    = HttpVersionT::HTTP_1_1;
    HttpRequestTypeT        request_type    = HttpRequestTypeT::GET;
    std::string             url;
    GpHttpHeaders           headers;
    GpBytesArray            body;
};

GpHttpRequest::GpHttpRequest (const GpHttpRequest& aRequest):
GpReflectObject(aRequest),
http_version(aRequest.http_version),
request_type(aRequest.request_type),
url(aRequest.url),
headers(aRequest.headers),
body(aRequest.body)
{
}

GpHttpRequest::GpHttpRequest (GpHttpRequest&& aRequest) noexcept:
GpReflectObject(std::move(aRequest)),
http_version(std::move(aRequest.http_version)),
request_type(std::move(aRequest.request_type)),
url(std::move(aRequest.url)),
headers(std::move(aRequest.headers)),
body(std::move(aRequest.body))
{
}

GpHttpRequest::GpHttpRequest
(
    const HttpVersionTE     aHttpVersion,
    const HttpRequestTypeTE aRequestType,
    std::string_view        aUrl,
    const GpHttpHeaders&    aHeaders,
    GpBytesArray&&          aBody
):
http_version(aHttpVersion),
request_type(aRequestType),
url(aUrl),
headers(aHeaders),
body(std::move(aBody))
{
}

std::string GpHttpRequest::ToString (void) const
{
    //TODO: implement
    return "\n\n\n[GpHttpRequest::ToString]: NOT IMPLEMENTED";
}

}//namespace GPlatform

#pragma once

#include "../Headers/GpHttpHeaders.hpp"
#include "../Url/GpUrl.hpp"
#include "../Enums/GpHttpVersion.hpp"
#include "../Enums/GpHttpRequestType.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestNoBodyDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpHttpRequestNoBodyDesc)
    REFLECT_DECLARE(u8"4b00f15a-7ce1-415f-be93-cba741adbe09"_uuid)

public:
    using HttpVersionT      = GpHttpVersion;
    using HttpVersionTE     = HttpVersionT::EnumT;

    using HttpRequestTypeT  = GpHttpRequestType;
    using HttpRequestTypeTE = HttpRequestTypeT::EnumT;

public:
    inline                  GpHttpRequestNoBodyDesc     (void) noexcept = default;
    inline                  GpHttpRequestNoBodyDesc     (const GpHttpRequestNoBodyDesc& aRequest);
    inline                  GpHttpRequestNoBodyDesc     (GpHttpRequestNoBodyDesc&& aRequest) noexcept;
    inline                  GpHttpRequestNoBodyDesc     (const HttpVersionTE        aHttpVersion,
                                                         const HttpRequestTypeTE    aRequestType,
                                                         GpUrl                      aUrl,
                                                         GpHttpHeaders              aHeaders);
    virtual                 ~GpHttpRequestNoBodyDesc    (void) noexcept override final;

    void                    SetHttpVersion              (const size_t aMajor,
                                                         const size_t aMinor);

public:
    HttpVersionT            http_version    = HttpVersionT::HTTP_1_1;
    HttpRequestTypeT        request_type    = HttpRequestTypeT::GET;
    GpUrl                   url;
    GpHttpHeaders           headers;
};

GpHttpRequestNoBodyDesc::GpHttpRequestNoBodyDesc (const GpHttpRequestNoBodyDesc& aRequest):
GpReflectObject(aRequest),
http_version(GpReflectUtils::SCopyValue(aRequest.http_version)),
request_type(GpReflectUtils::SCopyValue(aRequest.request_type)),
url         (GpReflectUtils::SCopyValue(aRequest.url)),
headers     (GpReflectUtils::SCopyValue(aRequest.headers))
{
}

GpHttpRequestNoBodyDesc::GpHttpRequestNoBodyDesc (GpHttpRequestNoBodyDesc&& aRequest) noexcept:
GpReflectObject(std::move(aRequest)),
http_version(std::move(aRequest.http_version)),
request_type(std::move(aRequest.request_type)),
url         (std::move(aRequest.url)),
headers     (std::move(aRequest.headers))
{
}

GpHttpRequestNoBodyDesc::GpHttpRequestNoBodyDesc
(
    const HttpVersionTE     aHttpVersion,
    const HttpRequestTypeTE aRequestType,
    GpUrl                   aUrl,
    GpHttpHeaders           aHeaders
):
http_version(std::move(aHttpVersion)),
request_type(std::move(aRequestType)),
url         (std::move(aUrl)),
headers     (std::move(aHeaders))
{
}

}//namespace GPlatform

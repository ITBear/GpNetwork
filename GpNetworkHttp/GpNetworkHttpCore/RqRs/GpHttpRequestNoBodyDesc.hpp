#pragma once

#include "../Headers/GpHttpHeaders.hpp"
#include "../Url/GpUrl.hpp"
#include "../Enums/GpHttpVersion.hpp"
#include "../Enums/GpHttpRequestType.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestNoBodyDesc
{
public:
    CLASS_DD(GpHttpRequestNoBodyDesc)

public:
    using HttpVersionT      = GpHttpVersion;
    using HttpVersionTE     = HttpVersionT::EnumT;

    using HttpRequestTypeT  = GpHttpRequestType;
    using HttpRequestTypeTE = HttpRequestTypeT::EnumT;

public:
    inline              GpHttpRequestNoBodyDesc     (void) noexcept = default;
    inline              GpHttpRequestNoBodyDesc     (const GpHttpRequestNoBodyDesc& aRequest);
    inline              GpHttpRequestNoBodyDesc     (GpHttpRequestNoBodyDesc&& aRequest) noexcept;
    inline              GpHttpRequestNoBodyDesc     (HttpVersionTE      aHttpVersion,
                                                     HttpRequestTypeTE  aRequestType,
                                                     GpUrl              aUrl,
                                                     GpHttpHeaders      aHeaders);
                        ~GpHttpRequestNoBodyDesc    (void) noexcept;

    void                Clear                       (void);

    void                SetHttpVersion              (size_t aMajor,
                                                     size_t aMinor);

public:
    HttpVersionT        http_version    = HttpVersionT::HTTP_1_1;
    HttpRequestTypeT    request_type    = HttpRequestTypeT::HTTP_GET;
    GpUrl               url;
    GpHttpHeaders       headers;
};

GpHttpRequestNoBodyDesc::GpHttpRequestNoBodyDesc (const GpHttpRequestNoBodyDesc& aRequest):
http_version{aRequest.http_version},
request_type{aRequest.request_type},
url         {aRequest.url},
headers     {aRequest.headers}
{
}

GpHttpRequestNoBodyDesc::GpHttpRequestNoBodyDesc (GpHttpRequestNoBodyDesc&& aRequest) noexcept:
http_version{std::move(aRequest.http_version)},
request_type{std::move(aRequest.request_type)},
url         {std::move(aRequest.url)},
headers     {std::move(aRequest.headers)}
{
}

GpHttpRequestNoBodyDesc::GpHttpRequestNoBodyDesc
(
    const HttpVersionTE     aHttpVersion,
    const HttpRequestTypeTE aRequestType,
    GpUrl                   aUrl,
    GpHttpHeaders           aHeaders
):
http_version{std::move(aHttpVersion)},
request_type{std::move(aRequestType)},
url         {std::move(aUrl)},
headers     {std::move(aHeaders)}
{
}

}// namespace GPlatform

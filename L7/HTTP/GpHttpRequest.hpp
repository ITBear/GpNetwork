#pragma once

#include "GpHttpVersion.hpp"
#include "GpHttpRequestType.hpp"
#include "GpHttpHeaders.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequest final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpHttpRequest)
    TYPE_STRUCT_DECLARE("4b00f15a-7ce1-415f-be93-cba741adbe09"_sv)

    using HttpVersionT      = GpHttpVersion;
    using HttpVersionTE     = HttpVersionT::EnumT;

    using HttpRequestTypeT  = GpHttpRequestType;
    using HttpRequestTypeTE = HttpRequestTypeT::EnumT;

public:
                                GpHttpRequest   (void) noexcept;
                                GpHttpRequest   (const GpHttpRequest& aRequest);
                                GpHttpRequest   (GpHttpRequest&& aRequest) noexcept;
                                GpHttpRequest   (const HttpVersionTE        aHttpVersion,
                                                 const HttpRequestTypeTE    aRequestType,
                                                 std::string_view           aUrl,
                                                 const GpHttpHeaders&       aHeaders,
                                                 GpBytesArray&&             aBody);
                                ~GpHttpRequest  (void) noexcept;

    void                        SetHttpVersion  (const size_t aMajor, const size_t aMinor);

public:
    HttpVersionT                http_version    = HttpVersionT::HTTP_1_1;
    HttpRequestTypeT            request_type    = HttpRequestTypeT::GET;
    std::string                 url;
    GpHttpHeaders               headers;
    GpBytesArray                body;
};

}//namespace GPlatform

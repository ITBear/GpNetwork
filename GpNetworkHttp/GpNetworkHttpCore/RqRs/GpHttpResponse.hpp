#pragma once

#include "../Body/GpHttpBodyPayload.hpp"
#include "../Exceptions/GpHttpException.hpp"
#include "GpHttpRequestNoBodyDesc.hpp"
#include "GpHttpResponseNoBodyDesc.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpResponse
{
public:
    CLASS_DD(GpHttpResponse)

public:
                                GpHttpResponse      (void) noexcept = default;
                                GpHttpResponse      (const GpHttpResponse& aResponse) = delete;
    inline                      GpHttpResponse      (GpHttpResponse&& aResponse) noexcept;
    inline                      GpHttpResponse      (GpHttpResponseNoBodyDesc   aResponseNoBodyDesc) noexcept;
    inline                      GpHttpResponse      (GpHttpResponseNoBodyDesc   aResponseNoBodyDesc,
                                                     GpHttpBodyPayload::SP      aBody) noexcept;
                                ~GpHttpResponse     (void) noexcept;

    static GpHttpResponse::SP   SFromException      (const GpHttpException&         aHttpEx,
                                                     const GpHttpRequestNoBodyDesc& aRqDesc);
    static GpHttpResponse::SP   SFromException      (const GpException&             aEx,
                                                     const GpHttpRequestNoBodyDesc& aRqDesc);
    static GpHttpResponse::SP   SFromException      (const std::exception&          aEx,
                                                     const GpHttpRequestNoBodyDesc& aRqDesc);

public:
    GpHttpResponseNoBodyDesc    iResponseNoBody;
    GpHttpBodyPayload::SP       iBody;
};

GpHttpResponse::GpHttpResponse (GpHttpResponse&& aResponse) noexcept:
iResponseNoBody(std::move(aResponse.iResponseNoBody)),
iBody          (std::move(aResponse.iBody))
{
}

GpHttpResponse::GpHttpResponse (GpHttpResponseNoBodyDesc aResponseNoBodyDesc) noexcept:
iResponseNoBody(std::move(aResponseNoBodyDesc))
{
}

GpHttpResponse::GpHttpResponse
(
    GpHttpResponseNoBodyDesc    aResponseNoBodyDesc,
    GpHttpBodyPayload::SP       aBody
) noexcept:
iResponseNoBody(std::move(aResponseNoBodyDesc)),
iBody          (std::move(aBody))
{
}

}//namespace GPlatform

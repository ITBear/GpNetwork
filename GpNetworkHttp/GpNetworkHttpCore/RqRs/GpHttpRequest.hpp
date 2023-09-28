#pragma once

#include "GpHttpRequestNoBodyDesc.hpp"
#include "../Body/GpHttpBodyPayload.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequest
{
public:
    CLASS_DD(GpHttpRequest)

public:
                                GpHttpRequest   (void) noexcept = default;
                                GpHttpRequest   (const GpHttpRequest& aRequest) = delete;
    inline                      GpHttpRequest   (GpHttpRequest&& aRequest) noexcept;
    inline                      GpHttpRequest   (GpHttpRequestNoBodyDesc    aRequestNoBodyDesc) noexcept;
    inline                      GpHttpRequest   (GpHttpRequestNoBodyDesc    aRequestNoBodyDesc,
                                                 GpHttpBodyPayload::SP      aBody) noexcept;
                                ~GpHttpRequest  (void) noexcept;

public:
    GpHttpRequestNoBodyDesc     iRequestNoBody;
    GpHttpBodyPayload::SP       iBody;
};

GpHttpRequest::GpHttpRequest (GpHttpRequest&& aRequest) noexcept:
iRequestNoBody(std::move(aRequest.iRequestNoBody)),
iBody         (std::move(aRequest.iBody))
{
}

GpHttpRequest::GpHttpRequest (GpHttpRequestNoBodyDesc aRequestNoBodyDesc) noexcept:
iRequestNoBody(std::move(aRequestNoBodyDesc))
{
}

GpHttpRequest::GpHttpRequest
(
    GpHttpRequestNoBodyDesc aRequestNoBodyDesc,
    GpHttpBodyPayload::SP   aBody
) noexcept:
iRequestNoBody(std::move(aRequestNoBodyDesc)),
iBody         (std::move(aBody))
{
}

}//namespace GPlatform

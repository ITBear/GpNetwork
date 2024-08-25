#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpRequestNoBodyDesc.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayload.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequest
{
public:
    CLASS_DD(GpHttpRequest)

    enum class SerializeRes
    {
        WRITE_HEADERS_ONLY,
        WRITE_HEADERS_AND_BODY
    };

    using RequestTypeToStrT = std::array<std::string, GpHttpRequestType::SCount()>;

public:
                                    GpHttpRequest   (void) noexcept = default;
                                    GpHttpRequest   (const GpHttpRequest& aRequest) = delete;
    inline                          GpHttpRequest   (GpHttpRequest&& aRequest) noexcept;
    inline                          GpHttpRequest   (GpHttpRequestNoBodyDesc    aRequestNoBodyDesc) noexcept;
    inline                          GpHttpRequest   (GpHttpRequestNoBodyDesc    aRequestNoBodyDesc,
                                                     GpHttpBodyPayload::SP      aBody) noexcept;
                                    ~GpHttpRequest  (void) noexcept;

    static SerializeRes             SSerialize      (GpHttpRequest& aHttpRequest,
                                                     GpByteWriter&  aWriter);

public:
    GpHttpRequestNoBodyDesc         iRequestNoBody;
    GpHttpBodyPayload::SP           iBody;

    static const RequestTypeToStrT  sRequestTypeToStr;
};

GpHttpRequest::GpHttpRequest (GpHttpRequest&& aRequest) noexcept:
iRequestNoBody{std::move(aRequest.iRequestNoBody)},
iBody         {std::move(aRequest.iBody)}
{
}

GpHttpRequest::GpHttpRequest (GpHttpRequestNoBodyDesc aRequestNoBodyDesc) noexcept:
iRequestNoBody{std::move(aRequestNoBodyDesc)}
{
}

GpHttpRequest::GpHttpRequest
(
    GpHttpRequestNoBodyDesc aRequestNoBodyDesc,
    GpHttpBodyPayload::SP   aBody
) noexcept:
iRequestNoBody{std::move(aRequestNoBodyDesc)},
iBody         {std::move(aBody)}
{
}

}// namespace GPlatform

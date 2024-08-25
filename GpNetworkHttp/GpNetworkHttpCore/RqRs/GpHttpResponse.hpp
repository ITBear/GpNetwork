#pragma once

#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayload.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Exceptions/GpHttpException.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpResponseNoBodyDesc.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpResponse
{
public:
    CLASS_DD(GpHttpResponse)

    enum class SerializeRes
    {
        WRITE_HEADERS_ONLY,
        WRITE_HEADERS_AND_BODY
    };

public:
                                GpHttpResponse  (void) noexcept = default;
                                GpHttpResponse  (const GpHttpResponse& aResponse) = delete;
    inline                      GpHttpResponse  (GpHttpResponse&& aResponse) noexcept;
    inline                      GpHttpResponse  (GpHttpResponseNoBodyDesc   aResponseNoBodyDesc) noexcept;
    inline                      GpHttpResponse  (GpHttpResponseNoBodyDesc   aResponseNoBodyDesc,
                                                 GpHttpBodyPayload::SP      aBody) noexcept;
                                ~GpHttpResponse (void) noexcept;



    static GpHttpResponse::SP   SFromException  (const GpHttpException& aHttpEx,
                                                 GpHttpVersion::EnumT   aHttpVersion);
    static GpHttpResponse::SP   SFromException  (const GpException&     aEx,
                                                 GpHttpVersion::EnumT   aHttpVersion);
    static GpHttpResponse::SP   SFromException  (const std::exception&  aEx,
                                                 GpHttpVersion::EnumT   aHttpVersion);

    static SerializeRes         SSerialize      (const GpHttpResponse&  aHttpResponse,
                                                 GpByteWriter&          aWriter);

public:
    GpHttpResponseNoBodyDesc    iResponseNoBody;
    GpHttpBodyPayload::SP       iBody;
};

GpHttpResponse::GpHttpResponse (GpHttpResponse&& aResponse) noexcept:
iResponseNoBody{std::move(aResponse.iResponseNoBody)},
iBody          {std::move(aResponse.iBody)}
{
}

GpHttpResponse::GpHttpResponse (GpHttpResponseNoBodyDesc aResponseNoBodyDesc) noexcept:
iResponseNoBody{std::move(aResponseNoBodyDesc)}
{
}

GpHttpResponse::GpHttpResponse
(
    GpHttpResponseNoBodyDesc    aResponseNoBodyDesc,
    GpHttpBodyPayload::SP       aBody
) noexcept:
iResponseNoBody{std::move(aResponseNoBodyDesc)},
iBody          {std::move(aBody)}
{
}

}// namespace GPlatform

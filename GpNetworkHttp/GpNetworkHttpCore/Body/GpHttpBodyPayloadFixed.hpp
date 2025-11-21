#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayload.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpBodyPayloadFixed final: public GpHttpBodyPayload
{
public:
    CLASS_DD(GpHttpBodyPayloadFixed)

public:
                    GpHttpBodyPayloadFixed  (void) noexcept = default;
    inline          GpHttpBodyPayloadFixed  (const GpByteArray& aData);
    inline          GpHttpBodyPayloadFixed  (GpByteArray&& aData) noexcept;
    virtual         ~GpHttpBodyPayloadFixed (void) noexcept override final;

    GpSpanByteRW    Data                    (void) noexcept {return iData;}
    GpSpanByteR     Data                    (void) const noexcept {return iData;}
    GpByteArray&    DataStorage             (void) noexcept {return iData;}

private:
    GpByteArray iData;
};

GpHttpBodyPayloadFixed::GpHttpBodyPayloadFixed (const GpByteArray& aData):
GpHttpBodyPayloadFixed(GpByteArray{aData})
{
}

GpHttpBodyPayloadFixed::GpHttpBodyPayloadFixed (GpByteArray&& aData) noexcept:
GpHttpBodyPayload
(
    GpHttpBodyPayloadType::FIXED_SIZE,
    std::size(aData)
),
iData{std::move(aData)}
{
}

}// namespace GPlatform

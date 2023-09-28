#pragma once

#include "GpHttpBodyPayload.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpBodyPayloadFixed final: public GpHttpBodyPayload
{
public:
    CLASS_DD(GpHttpBodyPayloadFixed)

public:
                        GpHttpBodyPayloadFixed  (void) noexcept = delete;
    inline              GpHttpBodyPayloadFixed  (const GpBytesArray& aData);
    inline              GpHttpBodyPayloadFixed  (GpBytesArray&& aData) noexcept;
    virtual             ~GpHttpBodyPayloadFixed (void) noexcept override final;

private:
    GpBytesArray        iData;
};

GpHttpBodyPayloadFixed::GpHttpBodyPayloadFixed (const GpBytesArray& aData):
GpHttpBodyPayloadFixed(GpBytesArray(aData))
{
}

GpHttpBodyPayloadFixed::GpHttpBodyPayloadFixed (GpBytesArray&& aData) noexcept:
GpHttpBodyPayload
(
    GpHttpBodyPayloadType::FIXED_SIZE,
    aData.size()
),
iData(std::move(aData))
{
}

}//namespace GPlatform

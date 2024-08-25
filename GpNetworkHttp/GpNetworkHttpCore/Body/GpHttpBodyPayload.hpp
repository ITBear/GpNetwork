#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadType.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpBodyPayload
{
public:
    CLASS_DD(GpHttpBodyPayload)

protected:
                                    GpHttpBodyPayload   (void) = default;
    inline                          GpHttpBodyPayload   (GpHttpBodyPayloadType::EnumT aType) noexcept;
    inline                          GpHttpBodyPayload   (GpHttpBodyPayloadType::EnumT   aType,
                                                         size_t                         aSize) noexcept;

public:
    virtual                         ~GpHttpBodyPayload  (void) noexcept;

    GpHttpBodyPayloadType::EnumT    Type                (void) const noexcept {return iType;}
    size_t                          Size                (void) const noexcept {return iSize;}

private:
    const GpHttpBodyPayloadType     iType = GpHttpBodyPayloadType::FIXED_SIZE;
    const size_t                    iSize = 0;
};

GpHttpBodyPayload::GpHttpBodyPayload (GpHttpBodyPayloadType::EnumT aType) noexcept:
iType{aType}
{
}

GpHttpBodyPayload::GpHttpBodyPayload
(
    GpHttpBodyPayloadType::EnumT    aType,
    const size_t                    aSize
) noexcept:
iType{aType},
iSize{aSize}
{
}

}// namespace GPlatform

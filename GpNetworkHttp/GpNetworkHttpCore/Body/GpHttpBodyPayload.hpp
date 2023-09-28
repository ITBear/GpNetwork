#pragma once

#include "GpHttpBodyPayloadType.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpBodyPayload
{
public:
    CLASS_DD(GpHttpBodyPayload)

protected:
                                    GpHttpBodyPayload   (void) = delete;
    inline                          GpHttpBodyPayload   (GpHttpBodyPayloadType::EnumT aType) noexcept;
    inline                          GpHttpBodyPayload   (GpHttpBodyPayloadType::EnumT   aType,
                                                         const size_t                   aSize) noexcept;

public:
    virtual                         ~GpHttpBodyPayload  (void) noexcept;

    GpHttpBodyPayloadType::EnumT    Type                (void) const noexcept {return iType.Value();}
    std::optional<size_t>           Size                (void) const noexcept {return iSize;}

private:
    const GpHttpBodyPayloadType     iType = GpHttpBodyPayloadType::FIXED_SIZE;
    const std::optional<size_t>     iSize;
};

GpHttpBodyPayload::GpHttpBodyPayload (GpHttpBodyPayloadType::EnumT aType) noexcept:
iType(aType)
{
}

GpHttpBodyPayload::GpHttpBodyPayload
(
    GpHttpBodyPayloadType::EnumT    aType,
    const size_t                    aSize
) noexcept:
iType(aType),
iSize(aSize)
{
}

}//namespace GPlatform

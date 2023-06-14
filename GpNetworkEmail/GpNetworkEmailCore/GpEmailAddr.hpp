#pragma once

#include "GpEmailHeaders.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpEmailAddr final: public GpReflectObject
{
public:
    CLASS_DD(GpEmailAddr)
    REFLECT_DECLARE(u8"464b10e2-1169-4c1e-9782-f4a71838840e"_uuid)

public:
                                GpEmailAddr         (void) noexcept;
                                GpEmailAddr         (const GpEmailAddr& aEmailAddr);
                                GpEmailAddr         (GpEmailAddr&& aEmailAddr) noexcept;
                                GpEmailAddr         (std::u8string_view aAddr);
                                GpEmailAddr         (std::u8string_view aName,
                                                     std::u8string_view aAddr);
    virtual                     ~GpEmailAddr        (void) noexcept override final;

public:
    std::u8string               name;
    std::u8string               addr;
};

}//namespace GPlatform

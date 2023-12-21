#pragma once

#include "../GpNetworkCore_global.hpp"

#include <GpCore2/GpReflection/GpReflectObject.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerCfgDesc: public GpReflectObject
{
public:
    CLASS_DD(GpIOEventPollerCfgDesc)
    REFLECT_DECLARE(u8"c3ed45bf-e3e1-4313-8e3e-395a4686f449"_uuid)

public:
                                    GpIOEventPollerCfgDesc  (void) noexcept = default;
    inline                          GpIOEventPollerCfgDesc  (const GpIOEventPollerCfgDesc& aDesc);
    inline                          GpIOEventPollerCfgDesc  (GpIOEventPollerCfgDesc&& aDesc) noexcept;
    virtual                         ~GpIOEventPollerCfgDesc (void) noexcept override;
};

GpIOEventPollerCfgDesc::GpIOEventPollerCfgDesc (const GpIOEventPollerCfgDesc& aDesc):
GpReflectObject(aDesc)
{
}

GpIOEventPollerCfgDesc::GpIOEventPollerCfgDesc (GpIOEventPollerCfgDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc))
{
}

}//namespace GPlatform

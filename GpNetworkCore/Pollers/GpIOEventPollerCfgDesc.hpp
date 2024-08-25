#pragma once

#include <GpNetwork/GpNetworkCore/GpNetworkCore_global.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerCfgDesc: public GpReflectObject
{
public:
    CLASS_DD(GpIOEventPollerCfgDesc)
    REFLECT_DECLARE("c3ed45bf-e3e1-4313-8e3e-395a4686f449"_uuid)

public:
                    GpIOEventPollerCfgDesc  (void) noexcept = default;
                    GpIOEventPollerCfgDesc  (const GpIOEventPollerCfgDesc& aDesc);
                    GpIOEventPollerCfgDesc  (GpIOEventPollerCfgDesc&& aDesc) noexcept;
    virtual         ~GpIOEventPollerCfgDesc (void) noexcept override;
};

}// namespace GPlatform

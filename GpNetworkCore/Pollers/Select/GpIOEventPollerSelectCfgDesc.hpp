#pragma once

#include "../GpIOEventPollerCfgDesc.hpp"

#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerSelectCfgDesc final: public GpIOEventPollerCfgDesc
{
public:
    CLASS_DD(GpIOEventPollerSelectCfgDesc)
    REFLECT_DECLARE("8c2bf762-c687-4402-81c7-acd86bd2c5ba"_uuid)

public:
                            GpIOEventPollerSelectCfgDesc    (void) noexcept = default;
    inline                  GpIOEventPollerSelectCfgDesc    (const GpIOEventPollerSelectCfgDesc& aDesc) noexcept;
    inline                  GpIOEventPollerSelectCfgDesc    (GpIOEventPollerSelectCfgDesc&& aDesc) noexcept;
    inline                  GpIOEventPollerSelectCfgDesc    (milliseconds_t aMaxStepTime) noexcept;
    virtual                 ~GpIOEventPollerSelectCfgDesc   (void) noexcept override final;

public:
    milliseconds_t          max_step_time;
};

GpIOEventPollerSelectCfgDesc::GpIOEventPollerSelectCfgDesc (const GpIOEventPollerSelectCfgDesc& aDesc) noexcept:
GpIOEventPollerCfgDesc(aDesc),
max_step_time (GpReflectUtils::SCopyValue(aDesc.max_step_time))
{
}

GpIOEventPollerSelectCfgDesc::GpIOEventPollerSelectCfgDesc (GpIOEventPollerSelectCfgDesc&& aDesc) noexcept:
GpIOEventPollerCfgDesc(std::move(aDesc)),
max_step_time(std::move(aDesc.max_step_time))
{
}

GpIOEventPollerSelectCfgDesc::GpIOEventPollerSelectCfgDesc (milliseconds_t  aMaxStepTime) noexcept:
max_step_time(std::move(aMaxStepTime))
{
}

}// namespace GPlatform

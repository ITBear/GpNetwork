#pragma once

#include "../GpIOEventPollerCfgDesc.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectObject.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectUtils.hpp"
#include "../../../../GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerEpollCfgDesc final: public GpIOEventPollerCfgDesc
{
public:
    CLASS_DD(GpIOEventPollerEpollCfgDesc)
    REFLECT_DECLARE(u8"72d4fb29-caa0-484f-9700-fcee4971e3c2"_uuid)

public:
                            GpIOEventPollerEpollCfgDesc     (void) noexcept = default;
    inline                  GpIOEventPollerEpollCfgDesc     (const GpIOEventPollerEpollCfgDesc& aDesc) noexcept;
    inline                  GpIOEventPollerEpollCfgDesc     (GpIOEventPollerEpollCfgDesc&& aDesc) noexcept;
    inline                  GpIOEventPollerEpollCfgDesc     (const milliseconds_t   aMaxStepTime,
                                                             const size_t           aMaxEventsCnt) noexcept;
    virtual                 ~GpIOEventPollerEpollCfgDesc    (void) noexcept override final;

public:
    milliseconds_t          max_step_time;
    size_t                  max_events_cnt = 0;
};

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (const GpIOEventPollerEpollCfgDesc& aDesc) noexcept:
GpIOEventPollerCfgDesc(aDesc),
max_step_time (GpReflectUtils::SCopyValue(aDesc.max_step_time)),
max_events_cnt(GpReflectUtils::SCopyValue(aDesc.max_events_cnt))
{
}

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (GpIOEventPollerEpollCfgDesc&& aDesc) noexcept:
GpIOEventPollerCfgDesc(std::move(aDesc)),
max_step_time (std::move(aDesc.max_step_time)),
max_events_cnt(std::move(aDesc.max_events_cnt))
{
}

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc
(
    const milliseconds_t    aMaxStepTime,
    const size_t            aMaxEventsCnt
) noexcept:
max_step_time (std::move(aMaxStepTime)),
max_events_cnt(std::move(aMaxEventsCnt))
{
}

}//namespace GPlatform

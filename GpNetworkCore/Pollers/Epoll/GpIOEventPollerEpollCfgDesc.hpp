#pragma once

#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCfgDesc.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>

namespace GPlatform {

#if defined(GP_OS_LINUX)

class GP_NETWORK_CORE_API GpIOEventPollerEpollCfgDesc final: public GpIOEventPollerCfgDesc
{
public:
    CLASS_DD(GpIOEventPollerEpollCfgDesc)
    REFLECT_DECLARE("72d4fb29-caa0-484f-9700-fcee4971e3c2"_uuid)

public:
                    GpIOEventPollerEpollCfgDesc     (void) noexcept = default;
                    GpIOEventPollerEpollCfgDesc     (const GpIOEventPollerEpollCfgDesc& aDesc) noexcept;
                    GpIOEventPollerEpollCfgDesc     (GpIOEventPollerEpollCfgDesc&& aDesc) noexcept;
                    GpIOEventPollerEpollCfgDesc     (milliseconds_t aMaxStepTime,
                                                     size_t         aMaxEventsCnt) noexcept;
    virtual         ~GpIOEventPollerEpollCfgDesc    (void) noexcept override final;

public:
    milliseconds_t  max_step_time   = 10.0_si_s;
    size_t          max_events_cnt  = 0;
};

#endif// #if defined(GP_OS_LINUX)

}// namespace GPlatform

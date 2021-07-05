#pragma once

#include "../../GpNetwork_global.hpp"

namespace GPlatform {

class GPNETWORK_API GpIOEventPollerEpollCfgDesc final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpIOEventPollerEpollCfgDesc)
    TYPE_STRUCT_DECLARE("72d4fb29-caa0-484f-9700-fcee4971e3c2"_sv)

public:
                            GpIOEventPollerEpollCfgDesc     (void) noexcept;
                            GpIOEventPollerEpollCfgDesc     (const GpIOEventPollerEpollCfgDesc& aDesc) noexcept;
                            GpIOEventPollerEpollCfgDesc     (GpIOEventPollerEpollCfgDesc&& aDesc) noexcept;
    virtual                 ~GpIOEventPollerEpollCfgDesc    (void) noexcept override final;

public:
    milliseconds_t          max_step_time;
    count_t                 max_events_cnt;
};

}//namespace GPlatform

#include "GpIOEventPollerEpollCfgDesc.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpIOEventPollerEpollCfgDesc, GP_MODULE_UUID)

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (void) noexcept
{
}

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (const GpIOEventPollerEpollCfgDesc& aDesc) noexcept:
max_step_time(aDesc.max_step_time),
max_events_cnt(aDesc.max_events_cnt)
{
}

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (GpIOEventPollerEpollCfgDesc&& aDesc) noexcept:
max_step_time(std::move(aDesc.max_step_time)),
max_events_cnt(std::move(aDesc.max_events_cnt))
{
}

GpIOEventPollerEpollCfgDesc::~GpIOEventPollerEpollCfgDesc (void) noexcept
{
}

void    GpIOEventPollerEpollCfgDesc::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(max_step_time);
    PROP(max_events_cnt);
}

}//namespace GPlatform

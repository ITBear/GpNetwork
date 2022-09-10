#include "GpIOEventPollerEpollCfgDesc.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpIOEventPollerEpollCfgDesc, GP_MODULE_UUID)

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (void) noexcept
{
}

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (const GpIOEventPollerEpollCfgDesc& aDesc) noexcept:
GpReflectObject(aDesc),
max_step_time (GpReflectUtils::SCopyValue(aDesc.max_step_time)),
max_events_cnt(GpReflectUtils::SCopyValue(aDesc.max_events_cnt))
{
}

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (GpIOEventPollerEpollCfgDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
max_step_time (std::move(aDesc.max_step_time)),
max_events_cnt(std::move(aDesc.max_events_cnt))
{
}

GpIOEventPollerEpollCfgDesc::~GpIOEventPollerEpollCfgDesc (void) noexcept
{
}

void    GpIOEventPollerEpollCfgDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(max_step_time);
    PROP(max_events_cnt);
}

}//namespace GPlatform

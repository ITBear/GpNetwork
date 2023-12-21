#include "GpIOEventPollerEpollCfgDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpIOEventPollerEpollCfgDesc, GP_MODULE_UUID)

GpIOEventPollerEpollCfgDesc::~GpIOEventPollerEpollCfgDesc (void) noexcept
{
}

void    GpIOEventPollerEpollCfgDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(max_step_time);
    PROP(max_events_cnt);
}

}//namespace GPlatform

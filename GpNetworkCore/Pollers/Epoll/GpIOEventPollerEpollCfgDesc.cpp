#include "GpIOEventPollerEpollCfgDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

#if defined(GP_OS_LINUX)

REFLECT_IMPLEMENT(GpIOEventPollerEpollCfgDesc, GP_MODULE_UUID)

GpIOEventPollerEpollCfgDesc::~GpIOEventPollerEpollCfgDesc (void) noexcept
{
}

void    GpIOEventPollerEpollCfgDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(max_step_time);
    PROP(max_events_cnt);
}

#endif// #if defined(GP_OS_LINUX)

}// namespace GPlatform

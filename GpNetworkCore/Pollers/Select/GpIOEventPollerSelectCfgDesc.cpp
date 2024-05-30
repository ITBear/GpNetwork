#include "GpIOEventPollerSelectCfgDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpIOEventPollerSelectCfgDesc, GP_MODULE_UUID)

GpIOEventPollerSelectCfgDesc::~GpIOEventPollerSelectCfgDesc (void) noexcept
{
}

void    GpIOEventPollerSelectCfgDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(max_step_time);
}

}// namespace GPlatform

#include "GpIOEventPollerCfgDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpIOEventPollerCfgDesc, GP_MODULE_UUID)

GpIOEventPollerCfgDesc::~GpIOEventPollerCfgDesc (void) noexcept
{
}

void    GpIOEventPollerCfgDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& /*aPropsOut*/)
{
}

}// namespace GPlatform

#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCfgDesc.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpIOEventPollerCfgDesc, GP_MODULE_UUID)

GpIOEventPollerCfgDesc::GpIOEventPollerCfgDesc (const GpIOEventPollerCfgDesc& aDesc):
GpReflectObject{aDesc}
{
}

GpIOEventPollerCfgDesc::GpIOEventPollerCfgDesc (GpIOEventPollerCfgDesc&& aDesc) noexcept:
GpReflectObject{std::move(aDesc)}
{
}

GpIOEventPollerCfgDesc::~GpIOEventPollerCfgDesc (void) noexcept
{
}

void    GpIOEventPollerCfgDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& /*aPropsOut*/)
{
}

}// namespace GPlatform

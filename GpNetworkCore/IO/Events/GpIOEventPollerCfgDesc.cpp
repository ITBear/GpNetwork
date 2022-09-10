#include "GpIOEventPollerCfgDesc.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpIOEventPollerCfgDesc, GP_MODULE_UUID)

GpIOEventPollerCfgDesc::GpIOEventPollerCfgDesc (void) noexcept
{
}

GpIOEventPollerCfgDesc::GpIOEventPollerCfgDesc (const GpIOEventPollerCfgDesc& aDesc):
GpReflectObject(aDesc),
epoll(GpReflectUtils::SCopyValue(aDesc.epoll))
{
}

GpIOEventPollerCfgDesc::GpIOEventPollerCfgDesc (GpIOEventPollerCfgDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
epoll(std::move(aDesc.epoll))
{
}

GpIOEventPollerCfgDesc::~GpIOEventPollerCfgDesc (void) noexcept
{
}

void    GpIOEventPollerCfgDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(epoll);
}

}//namespace GPlatform

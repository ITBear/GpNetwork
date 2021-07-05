#include "GpIOEventPollerCfgDesc.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpIOEventPollerCfgDesc, GP_MODULE_UUID)

GpIOEventPollerCfgDesc::GpIOEventPollerCfgDesc (void) noexcept
{
}

GpIOEventPollerCfgDesc::~GpIOEventPollerCfgDesc (void) noexcept
{
}

void    GpIOEventPollerCfgDesc::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(epoll);
}

}//namespace GPlatform

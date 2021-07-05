#include "GpHttpServerCfgDesc.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpHttpServerCfgDesc, GP_MODULE_UUID)

GpHttpServerCfgDesc::GpHttpServerCfgDesc (void) noexcept
{
}

GpHttpServerCfgDesc::~GpHttpServerCfgDesc (void) noexcept
{
}

void    GpHttpServerCfgDesc::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(listen_ip);
    PROP(listen_port);
    PROP(event_poller);
}

}//namespace GPlatform

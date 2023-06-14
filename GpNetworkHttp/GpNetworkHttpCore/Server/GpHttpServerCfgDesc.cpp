#include "GpHttpServerCfgDesc.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectManager.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpHttpServerCfgDesc, GP_MODULE_UUID)

GpHttpServerCfgDesc::~GpHttpServerCfgDesc (void) noexcept
{
}

void    GpHttpServerCfgDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(listen_ip);
    PROP(listen_port);
    PROP(event_poller);
}

}//namespace GPlatform

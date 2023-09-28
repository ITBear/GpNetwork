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
    PROP(listen_max_queue_size);
    PROP(event_poller_name);
    PROP(accept_socket_flags);
    PROP(listen_socket_flags);
}

}//namespace GPlatform

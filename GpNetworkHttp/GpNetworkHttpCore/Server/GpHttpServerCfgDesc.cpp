#include "GpHttpServerCfgDesc.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpHttpServerCfgDesc, GP_MODULE_UUID)

GpHttpServerCfgDesc::GpHttpServerCfgDesc (void) noexcept
{
}

GpHttpServerCfgDesc::GpHttpServerCfgDesc (const GpHttpServerCfgDesc& aDesc):
GpReflectObject(aDesc),
listen_ip(aDesc.listen_ip),
listen_port(aDesc.listen_port),
event_poller(aDesc.event_poller)
{
}

GpHttpServerCfgDesc::GpHttpServerCfgDesc (GpHttpServerCfgDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
listen_ip(std::move(aDesc.listen_ip)),
listen_port(std::move(aDesc.listen_port)),
event_poller(std::move(aDesc.event_poller))
{
}

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

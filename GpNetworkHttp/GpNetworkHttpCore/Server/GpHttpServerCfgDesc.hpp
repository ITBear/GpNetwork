#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectObject.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectUtils.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServerCfgDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpHttpServerCfgDesc)
    REFLECT_DECLARE(u8"082d98c8-f18f-4c33-b9c0-07f33a5acad9"_uuid)

public:
                        GpHttpServerCfgDesc     (void) noexcept = default;
    inline              GpHttpServerCfgDesc     (const GpHttpServerCfgDesc& aDesc);
    inline              GpHttpServerCfgDesc     (GpHttpServerCfgDesc&& aDesc) noexcept;
    virtual             ~GpHttpServerCfgDesc    (void) noexcept override final;

public:
    std::u8string       listen_ip;
    u_int_16            listen_port = 80;
    std::u8string       event_poller;
};

GpHttpServerCfgDesc::GpHttpServerCfgDesc (const GpHttpServerCfgDesc& aDesc):
GpReflectObject(aDesc),
listen_ip   (GpReflectUtils::SCopyValue(aDesc.listen_ip)),
listen_port (GpReflectUtils::SCopyValue(aDesc.listen_port)),
event_poller(GpReflectUtils::SCopyValue(aDesc.event_poller))
{
}

GpHttpServerCfgDesc::GpHttpServerCfgDesc (GpHttpServerCfgDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
listen_ip   (std::move(aDesc.listen_ip)),
listen_port (std::move(aDesc.listen_port)),
event_poller(std::move(aDesc.event_poller))
{
}

}//namespace GPlatform

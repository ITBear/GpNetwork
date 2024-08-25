#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../GpNetworkCore/Sockets/GpSocketFlags.hpp"

#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServerCfgDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpHttpServerCfgDesc)
    REFLECT_DECLARE("082d98c8-f18f-4c33-b9c0-07f33a5acad9"_uuid)

public:
                                GpHttpServerCfgDesc     (void) noexcept = default;
    inline                      GpHttpServerCfgDesc     (const GpHttpServerCfgDesc& aDesc);
    inline                      GpHttpServerCfgDesc     (GpHttpServerCfgDesc&& aDesc) noexcept;
    virtual                     ~GpHttpServerCfgDesc    (void) noexcept override final;

    inline GpHttpServerCfgDesc& operator=               (const GpHttpServerCfgDesc& aDesc) noexcept;
    inline GpHttpServerCfgDesc& operator=               (GpHttpServerCfgDesc&& aDesc) noexcept;

public:
    std::string                 listen_ip               = "127.0.0.1";
    u_int_16                    listen_port             = 80;
    u_int_16                    listen_max_queue_size   = 100;
    std::string                 event_poller_name;
    GpSocketFlags               accept_socket_flags;
    GpSocketFlags               listen_socket_flags;
};

GpHttpServerCfgDesc::GpHttpServerCfgDesc (const GpHttpServerCfgDesc& aDesc):
GpReflectObject{aDesc},
listen_ip            {GpReflectUtils::SCopyValue(aDesc.listen_ip)},
listen_port          {GpReflectUtils::SCopyValue(aDesc.listen_port)},
listen_max_queue_size{GpReflectUtils::SCopyValue(aDesc.listen_max_queue_size)},
event_poller_name    {GpReflectUtils::SCopyValue(aDesc.event_poller_name)},
accept_socket_flags  {GpReflectUtils::SCopyValue(aDesc.accept_socket_flags)},
listen_socket_flags  {GpReflectUtils::SCopyValue(aDesc.listen_socket_flags)}
{
}

GpHttpServerCfgDesc::GpHttpServerCfgDesc (GpHttpServerCfgDesc&& aDesc) noexcept:
GpReflectObject{std::move(aDesc)},
listen_ip            {std::move(aDesc.listen_ip)},
listen_port          {std::move(aDesc.listen_port)},
listen_max_queue_size{std::move(aDesc.listen_max_queue_size)},
event_poller_name    {std::move(aDesc.event_poller_name)},
accept_socket_flags  {std::move(aDesc.accept_socket_flags)},
listen_socket_flags  {std::move(aDesc.listen_socket_flags)}
{
}

GpHttpServerCfgDesc&    GpHttpServerCfgDesc::operator= (const GpHttpServerCfgDesc& aDesc) noexcept
{
    listen_ip               = GpReflectUtils::SCopyValue(aDesc.listen_ip);
    listen_port             = GpReflectUtils::SCopyValue(aDesc.listen_port);
    listen_max_queue_size   = GpReflectUtils::SCopyValue(aDesc.listen_max_queue_size);
    event_poller_name       = GpReflectUtils::SCopyValue(aDesc.event_poller_name);
    accept_socket_flags     = GpReflectUtils::SCopyValue(aDesc.accept_socket_flags);
    listen_socket_flags     = GpReflectUtils::SCopyValue(aDesc.listen_socket_flags);

    return *this;
}

GpHttpServerCfgDesc&    GpHttpServerCfgDesc::operator= (GpHttpServerCfgDesc&& aDesc) noexcept
{
    listen_ip               = std::move(aDesc.listen_ip);
    listen_port             = std::move(aDesc.listen_port);
    listen_max_queue_size   = std::move(aDesc.listen_max_queue_size);
    event_poller_name       = std::move(aDesc.event_poller_name);
    accept_socket_flags     = std::move(aDesc.accept_socket_flags);
    listen_socket_flags     = std::move(aDesc.listen_socket_flags);

    return *this;
}

}// namespace GPlatform

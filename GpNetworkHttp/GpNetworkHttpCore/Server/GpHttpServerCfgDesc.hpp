#pragma once

#include "../GpNetworkHttpCore_global.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServerCfgDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpHttpServerCfgDesc)
    REFLECT_DECLARE("082d98c8-f18f-4c33-b9c0-07f33a5acad9"_uuid)

public:
                        GpHttpServerCfgDesc     (void) noexcept;
                        GpHttpServerCfgDesc     (const GpHttpServerCfgDesc& aDesc);
                        GpHttpServerCfgDesc     (GpHttpServerCfgDesc&& aDesc) noexcept;
    virtual             ~GpHttpServerCfgDesc    (void) noexcept override final;


    std::string_view    ListenIP                (void) const noexcept {return listen_ip;}
    u_int_16            ListenPort              (void) const noexcept {return listen_port;}
    std::string_view    EventPoller             (void) const noexcept {return event_poller;}

private:
    std::string         listen_ip;
    u_int_16            listen_port = 80;
    std::string         event_poller;
};

}//namespace GPlatform

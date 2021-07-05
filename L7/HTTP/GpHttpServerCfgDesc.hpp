#pragma once

#include "../../GpNetwork_global.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpServerCfgDesc final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpHttpServerCfgDesc)
    TYPE_STRUCT_DECLARE("082d98c8-f18f-4c33-b9c0-07f33a5acad9"_sv)

public:
                            GpHttpServerCfgDesc     (void) noexcept;
    virtual                 ~GpHttpServerCfgDesc    (void) noexcept override final;


    std::string_view        ListenIP                (void) const noexcept {return listen_ip;}
    u_int_16                ListenPort              (void) const noexcept {return listen_port;}
    std::string_view        EventPoller             (void) const noexcept {return event_poller;}

private:
    std::string             listen_ip;
    u_int_16                listen_port = 80;
    std::string             event_poller;
};

}//namespace GPlatform

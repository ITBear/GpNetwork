#pragma once

#include "GpIOEventPollerEpollCfgDesc.hpp"

namespace GPlatform {

class GPNETWORK_API GpIOEventPollerCfgDesc final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpIOEventPollerCfgDesc)
    TYPE_STRUCT_DECLARE("236316b0-244f-4f2e-8ed5-1b2f8298d728"_sv)

public:
                            GpIOEventPollerCfgDesc  (void) noexcept;
    virtual                 ~GpIOEventPollerCfgDesc (void) noexcept override final;

public:
    GpIOEventPollerEpollCfgDesc::SP epoll;
};

}//namespace GPlatform

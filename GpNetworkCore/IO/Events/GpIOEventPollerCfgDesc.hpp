#pragma once

#include "GpIOEventPollerEpollCfgDesc.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerCfgDesc final: public GpReflectObject
{
public:
    CLASS_DECLARE_DEFAULTS(GpIOEventPollerCfgDesc)
    REFLECT_DECLARE("236316b0-244f-4f2e-8ed5-1b2f8298d728"_uuid)

public:
                            GpIOEventPollerCfgDesc  (void) noexcept;
                            GpIOEventPollerCfgDesc  (const GpIOEventPollerCfgDesc& aDesc);
                            GpIOEventPollerCfgDesc  (GpIOEventPollerCfgDesc&& aDesc) noexcept;
    virtual                 ~GpIOEventPollerCfgDesc (void) noexcept override final;

public:
    GpIOEventPollerEpollCfgDesc::SP epoll;
};

}//namespace GPlatform

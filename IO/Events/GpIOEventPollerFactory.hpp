#pragma once

#include "../../GpNetwork_global.hpp"

namespace GPlatform {

class GpIOEventPoller;

class GPNETWORK_API GpIOEventPollerFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpIOEventPollerFactory)
    CLASS_DECLARE_DEFAULTS(GpIOEventPollerFactory)

protected:
                                    GpIOEventPollerFactory  (void) noexcept {}

public:
    virtual                         ~GpIOEventPollerFactory (void) noexcept {}

    virtual GpSP<GpIOEventPoller>   NewInstance             (std::string_view       aName,
                                                             GpTaskFiberBarrier::SP aStartBarrier) const = 0;
};

}//namespace GPlatform

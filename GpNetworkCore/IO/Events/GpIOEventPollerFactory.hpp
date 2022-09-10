#pragma once

#include "../../GpNetworkCore_global.hpp"

namespace GPlatform {

class GpIOEventPoller;

class GP_NETWORK_CORE_API GpIOEventPollerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPollerFactory)
    CLASS_DECLARE_DEFAULTS(GpIOEventPollerFactory)

protected:
                                    GpIOEventPollerFactory  (void) noexcept = default;

public:
    virtual                         ~GpIOEventPollerFactory (void) noexcept = default;

    virtual GpSP<GpIOEventPoller>   NewInstance             (std::string    aName,
                                                             GpItcPromise&& aStartPromise) const = 0;
};

}//namespace GPlatform

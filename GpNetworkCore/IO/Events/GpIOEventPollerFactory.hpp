#pragma once

#include "../../GpNetworkCore_global.hpp"

#include "GpIOEventPoller.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPollerFactory)
    CLASS_DD(GpIOEventPollerFactory)

    using StartItcPromiseT  = typename GpIOEventPoller::StartItcPromiseT;
    using StartItcFutureT   = typename GpIOEventPoller::StartItcFutureT;
    using StartItcResultT   = typename GpIOEventPoller::StartItcResultT;

protected:
                                    GpIOEventPollerFactory  (void) noexcept = default;

public:
    virtual                         ~GpIOEventPollerFactory (void) noexcept = default;

    virtual GpSP<GpIOEventPoller>   NewInstance             (std::u8string      aName,
                                                             StartItcPromiseT&& aStartPromise) const = 0;
};

}//namespace GPlatform

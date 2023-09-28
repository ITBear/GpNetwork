#pragma once

#include "GpIOEventPoller.hpp"

namespace GPlatform {

class GpIOEventPollerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPollerFactory)
    CLASS_DD(GpIOEventPollerFactory)

protected:
                                    GpIOEventPollerFactory  (void) noexcept = default;

public:
    virtual                         ~GpIOEventPollerFactory (void) noexcept = default;

    virtual GpIOEventPoller::SP     NewInstance             (std::u8string aName) const = 0;
};

}//namespace GPlatform

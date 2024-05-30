#pragma once

#include "../GpIOEventPollerFactory.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerSelectFactory final: public GpIOEventPollerFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpIOEventPollerSelectFactory)
    CLASS_DD(GpIOEventPollerSelectFactory)

public:
    inline                          GpIOEventPollerSelectFactory    (milliseconds_t aMaxStepTime) noexcept;
    virtual                         ~GpIOEventPollerSelectFactory   (void) noexcept override final;

    virtual GpSP<GpIOEventPoller>   NewInstance                     (std::string aName) const override final;

private:
    const milliseconds_t            iMaxStepTime;
};

GpIOEventPollerSelectFactory::GpIOEventPollerSelectFactory (milliseconds_t aMaxStepTime) noexcept:
iMaxStepTime(aMaxStepTime)
{
}

}// namespace GPlatform

#pragma once

#include "GpIOEventPollerFactory.hpp"

#if defined(GP_OS_LINUX)

namespace GPlatform {

class GPNETWORK_API GpIOEventPollerEpollFactory final: public GpIOEventPollerFactory
{
public:
    CLASS_REMOVE_CTRS(GpIOEventPollerEpollFactory)
    CLASS_DECLARE_DEFAULTS(GpIOEventPollerEpollFactory)

public:
                                    GpIOEventPollerEpollFactory     (const milliseconds_t   aMaxStepTime,
                                                                     const count_t          aMaxEventsCnt) noexcept;
    virtual                         ~GpIOEventPollerEpollFactory    (void) noexcept override final;

    virtual GpSP<GpIOEventPoller>   NewInstance                     (GpTaskFiberBarrier::SP aStartBarrier) const override final;

private:
    const milliseconds_t            iMaxStepTime;
    const count_t                   iMaxEventsCnt;
};

}//namespace GPlatform

#endif//#if defined(GP_OS_LINUX)

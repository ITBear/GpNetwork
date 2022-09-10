#pragma once

#include "GpIOEventPollerFactory.hpp"

#if defined(GP_OS_LINUX)

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerEpollFactory final: public GpIOEventPollerFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpIOEventPollerEpollFactory)
    CLASS_DECLARE_DEFAULTS(GpIOEventPollerEpollFactory)

public:
    inline                          GpIOEventPollerEpollFactory     (const milliseconds_t   aMaxStepTime,
                                                                     const count_t          aMaxEventsCnt) noexcept;
    virtual                         ~GpIOEventPollerEpollFactory    (void) noexcept override final;

    virtual GpSP<GpIOEventPoller>   NewInstance                     (std::string    aName,
                                                                     GpItcPromise&& aStartPromise) const override final;

private:
    const milliseconds_t            iMaxStepTime;
    const count_t                   iMaxEventsCnt;
};

GpIOEventPollerEpollFactory::GpIOEventPollerEpollFactory
(
    const milliseconds_t    aMaxStepTime,
    const count_t           aMaxEventsCnt
) noexcept:
iMaxStepTime(aMaxStepTime),
iMaxEventsCnt(aMaxEventsCnt)
{
}

}//namespace GPlatform

#endif//#if defined(GP_OS_LINUX)

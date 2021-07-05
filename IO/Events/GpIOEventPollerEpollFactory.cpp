#include "GpIOEventPollerEpollFactory.hpp"

#if defined(GP_OS_LINUX)

#include "GpIOEventPollerEpoll.hpp"

namespace GPlatform {

GpIOEventPollerEpollFactory::GpIOEventPollerEpollFactory
(
    const milliseconds_t    aMaxStepTime,
    const count_t           aMaxEventsCnt
) noexcept:
iMaxStepTime(aMaxStepTime),
iMaxEventsCnt(aMaxEventsCnt)
{
}

GpIOEventPollerEpollFactory::~GpIOEventPollerEpollFactory (void) noexcept
{
}

GpIOEventPoller::SP GpIOEventPollerEpollFactory::NewInstance
(
    std::string_view        aName,
    GpTaskFiberBarrier::SP  aStartBarrier
) const
{
    GpIOEventPollerEpoll::SP epollSP = MakeSP<GpIOEventPollerEpoll>(aName, std::move(aStartBarrier));
    epollSP->Configure(iMaxStepTime, iMaxEventsCnt);
    return epollSP;
}

}//namespace GPlatform

#endif//#if defined(GP_OS_LINUX)

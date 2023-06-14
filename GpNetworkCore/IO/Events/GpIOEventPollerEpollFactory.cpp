#include "GpIOEventPollerEpollFactory.hpp"

#if defined(GP_OS_LINUX)

#include "GpIOEventPollerEpoll.hpp"

namespace GPlatform {

GpIOEventPollerEpollFactory::~GpIOEventPollerEpollFactory (void) noexcept
{
}

GpIOEventPoller::SP GpIOEventPollerEpollFactory::NewInstance
(
    std::u8string       aName,
    StartItcPromiseT&&  aStartPromise
) const
{
    GpIOEventPollerEpoll::SP epollSP = MakeSP<GpIOEventPollerEpoll>
    (
        std::move(aName),
        std::move(aStartPromise)
    );

    epollSP.V().Configure(iMaxStepTime, iMaxEventsCnt);
    return epollSP;
}

}//namespace GPlatform

#endif//#if defined(GP_OS_LINUX)

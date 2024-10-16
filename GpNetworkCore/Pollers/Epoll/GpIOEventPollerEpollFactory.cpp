#include <GpNetwork/GpNetworkCore/Pollers/Epoll/GpIOEventPollerEpollFactory.hpp>

#if defined(GP_OS_LINUX)

#include <GpNetwork/GpNetworkCore/Pollers/Epoll/GpIOEventPollerEpoll.hpp>

namespace GPlatform {

GpIOEventPollerEpollFactory::~GpIOEventPollerEpollFactory (void) noexcept
{
}

GpIOEventPoller::SP GpIOEventPollerEpollFactory::NewInstance (std::string aName) const
{
    GpIOEventPollerEpoll::SP epollSP = MakeSP<GpIOEventPollerEpoll>(std::move(aName));

    epollSP.V().Configure(iMaxStepTime, iMaxEventsCnt);

    return epollSP;
}

}// namespace GPlatform

#endif// #if defined(GP_OS_LINUX)

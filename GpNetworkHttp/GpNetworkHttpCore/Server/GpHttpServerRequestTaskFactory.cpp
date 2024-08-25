#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Server/GpHttpServerRequestTaskFactory.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Server/GpHttpServerRequestTask.hpp>

namespace GPlatform {

GpHttpServerRequestTaskFactory::GpHttpServerRequestTaskFactory (GpHttpRouter::SP aRouter) noexcept:
iRouter{std::move(aRouter)}
{
}

GpHttpServerRequestTaskFactory::~GpHttpServerRequestTaskFactory (void) noexcept
{
}

GpTcpServerTask::SP GpHttpServerRequestTaskFactory::NewInstance
(
    GpSocketTCP::SP     aSocketTCP,
    GpIOEventPollerIdx  aIOEventPollerIdx
) const
{
    return MakeSP<GpHttpServerRequestTask>
    (
        std::move(aSocketTCP),
        aIOEventPollerIdx,
        iRouter
    );
}

}// namespace GPlatform

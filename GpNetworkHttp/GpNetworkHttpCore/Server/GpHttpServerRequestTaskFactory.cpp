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

GpSocketTask::SP    GpHttpServerRequestTaskFactory::NewInstance (GpSocket::UP aSocketUP) const
{
    VERIFY
    (
        aSocketUP->Protocol() == GpSocketProtocol::TCP,
        "Socket must be TCP"
    );

    return MakeSP<GpHttpServerRequestTask>
    (
        std::unique_ptr<GpSocketTCP>{static_cast<GpSocketTCP*>(aSocketUP.release())},
        iRouter
    );
}

}// namespace GPlatform

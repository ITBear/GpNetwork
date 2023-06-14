#include "GpHttpServerBasicSocketTaskFactory.hpp"
#include "GpHttpServerBasicSocketTask.hpp"

namespace GPlatform {

GpHttpServerBasicSocketTaskFactory::GpHttpServerBasicSocketTaskFactory (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
GpHttpServerSocketTaskFactory(std::move(aRequestHandlerFactory))
{
}

GpHttpServerBasicSocketTaskFactory::~GpHttpServerBasicSocketTaskFactory (void) noexcept
{
}

GpSocketTask::SP    GpHttpServerBasicSocketTaskFactory::NewInstance
(
    std::u8string       aName,
    GpIOEventPoller::SP aIOPoller,
    GpSocket::SP        aSocket
) const
{
    return MakeSP<GpHttpServerBasicSocketTask>
    (
        std::move(aName),
        std::move(aIOPoller),
        std::move(aSocket),
        RequestHandlerFactory()
    );
}

}//namespace GPlatform

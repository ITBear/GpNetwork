#include "GpHttpServerNodeSocketTaskFactory.hpp"
#include "GpHttpServerNodeSocketTask.hpp"

namespace GPlatform {

GpHttpServerNodeSocketTaskFactory::GpHttpServerNodeSocketTaskFactory (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
GpHttpServerSocketTaskFactory(std::move(aRequestHandlerFactory))
{
}

GpHttpServerNodeSocketTaskFactory::~GpHttpServerNodeSocketTaskFactory (void) noexcept
{
}

GpSocketTask::SP    GpHttpServerNodeSocketTaskFactory::NewInstance
(
    std::string_view    aName,
    GpIOEventPoller::WP aIOPoller,
    GpSocket::SP        aSocket
) const
{
    return MakeSP<GpHttpServerNodeSocketTask>
    (
        aName,
        std::move(aIOPoller),
        std::move(aSocket),
        RequestHandlerFactory()
    );
}

}//namespace GPlatform

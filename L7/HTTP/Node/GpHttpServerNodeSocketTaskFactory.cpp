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

GpSocketTask::SP    GpHttpServerNodeSocketTaskFactory::NewInstance (GpIOEventPoller::WP aIOPooler,
                                                                    GpSocket::SP        aSocket) const
{
    return MakeSP<GpHttpServerNodeSocketTask>(std::move(aIOPooler),
                                              std::move(aSocket),
                                              RequestHandlerFactory());
}

}//namespace GPlatform

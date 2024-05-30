#include "GpHttpRequestSocketTaskFactory.hpp"
#include "GpHttpRequestSocketTask.hpp"

namespace GPlatform {

GpHttpRequestSocketTaskFactory::GpHttpRequestSocketTaskFactory (GpHttpRouter::SP aRouter) noexcept:
iRouter{std::move(aRouter)}
{
}

GpHttpRequestSocketTaskFactory::~GpHttpRequestSocketTaskFactory (void) noexcept
{
}

GpSingleSocketTask::SP  GpHttpRequestSocketTaskFactory::NewInstance
(
    GpSocket::SP                aSocket,
    const GpIOEventPollerIdx    aIOEventPollerIdx
) const
{
    return MakeSP<GpHttpRequestSocketTask>
    (
        std::move(aSocket),
        iRouter,
        aIOEventPollerIdx
    );
}

}// namespace GPlatform

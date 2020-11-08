#include "GpHttpServerNodeSocketTaskFactory.hpp"
#include "GpHttpServerNodeSocketTask.hpp"

namespace GPlatform {

GpHttpServerNodeSocketTaskFactory::GpHttpServerNodeSocketTaskFactory (void) noexcept
{
}

GpHttpServerNodeSocketTaskFactory::~GpHttpServerNodeSocketTaskFactory (void) noexcept
{
}

GpSocketTask::SP    GpHttpServerNodeSocketTaskFactory::NewInstance (GpIOEventPoller::WP aIOPooler,
                                                                    GpSocket::SP        aSocket) const
{
    return MakeSP<GpHttpServerNodeSocketTask>(std::move(aIOPooler),
                                              std::move(aSocket));
}

}//namespace GPlatform

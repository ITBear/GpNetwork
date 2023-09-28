#include "GpHttpRequestSocketTaskFactory.hpp"
#include "GpHttpRequestSocketTask.hpp"

namespace GPlatform {

GpHttpRequestSocketTaskFactory::~GpHttpRequestSocketTaskFactory (void) noexcept
{

}

GpSocketTask::SP    GpHttpRequestSocketTaskFactory::NewInstance (GpSocket::SP aSocket) const
{
    return MakeSP<GpHttpRequestSocketTask>(std::move(aSocket));
}

}//namespace GPlatform

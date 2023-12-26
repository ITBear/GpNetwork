#include "GpHttpRequestSocketTaskFactory.hpp"
#include "GpHttpRequestSocketTask.hpp"

namespace GPlatform {

GpHttpRequestSocketTaskFactory::~GpHttpRequestSocketTaskFactory (void) noexcept
{

}

GpSingleSocketTask::SP  GpHttpRequestSocketTaskFactory::NewInstance (GpSocket::SP aSocket) const
{
    return MakeSP<GpHttpRequestSocketTask>(std::move(aSocket));
}

}//namespace GPlatform

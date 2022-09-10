#include "GpHttpClientPool.hpp"

namespace GPlatform {

GpHttpClientPool::GpHttpClientPool (GpHttpClientFactory::SP aFactory) noexcept:
iFactory(std::move(aFactory))
{
}

GpHttpClientPool::~GpHttpClientPool (void) noexcept
{
}

void    GpHttpClientPool::PreInit (const size_t /*aCount*/)
{
    //NOP
}

GpHttpClient::SP    GpHttpClientPool::NewElement (GpSpinlock& /*aLocked*/)
{
    return iFactory.V().NewInstance();
}

void    GpHttpClientPool::OnClear (void) noexcept
{
    //NOP
}

bool    GpHttpClientPool::Validate (GpHttpClient::SP aHttpClient) noexcept
{
    if (aHttpClient.IsNULL())
    {
        return false;
    }

    return aHttpClient.Vn().IsValid();
}

}//namespace GPlatform

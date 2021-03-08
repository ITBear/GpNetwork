#include "GpHttpClientPool.hpp"

namespace GPlatform {

GpHttpClientPool::GpHttpClientPool (GpHttpClientFactory::SP aFactory) noexcept:
iFactory(std::move(aFactory))
{
}

GpHttpClientPool::~GpHttpClientPool (void) noexcept
{
}

void    GpHttpClientPool::PreInit (const count_t /*aCount*/)
{
    //NOP
}

GpHttpClient::SP    GpHttpClientPool::NewElement (void)
{
    return iFactory->NewInstance();
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

    return aHttpClient.VCn().IsValid();
}

}//namespace GPlatform
#include "GpSmtpClientPool.hpp"

namespace GPlatform {

GpSmtpClientPool::GpSmtpClientPool (GpSmtpClientFactory::SP aFactory) noexcept:
iFactory(std::move(aFactory))
{
}

GpSmtpClientPool::~GpSmtpClientPool (void) noexcept
{
}

void    GpSmtpClientPool::PreInit (const size_t /*aCount*/)
{
    //NOP
}

GpSmtpClient::SP    GpSmtpClientPool::NewElement (GpSpinlock& /*aLocked*/)
{
    return iFactory.V().NewInstance();
}

void    GpSmtpClientPool::OnClear (void) noexcept
{
    //NOP
}

bool    GpSmtpClientPool::Validate (GpSmtpClient::SP aSmtpClient) noexcept
{
    if (aSmtpClient.IsNULL())
    {
        return false;
    }

    return aSmtpClient.Vn().IsValid();
}

}//namespace GPlatform

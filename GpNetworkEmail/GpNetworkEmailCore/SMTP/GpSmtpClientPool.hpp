#pragma once

#include "GpSmtpClient.hpp"
#include "GpSmtpClientFactory.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpSmtpClientPool final: public GpElementsPool<GpSmtpClient::SP>
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSmtpClientPool)
    CLASS_DECLARE_DEFAULTS(GpSmtpClientPool)
    CLASS_TAG(THREAD_SAFE)

public:
                                    GpSmtpClientPool    (GpSmtpClientFactory::SP aFactory) noexcept;
                                    ~GpSmtpClientPool   (void) noexcept override final;

protected:
    virtual void                    PreInit             (const size_t aCount) override final;
    virtual GpSmtpClient::SP        NewElement          (GpSpinlock& aLocked) override final;
    virtual void                    OnClear             (void) noexcept override final;
    virtual bool                    Validate            (GpSmtpClient::SP aSmtpClient) noexcept override final;

private:
    GpSmtpClientFactory::SP         iFactory;
};

}//namespace GPlatform

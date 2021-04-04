#pragma once

#include "GpHttpClientFactory.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpClientPool final: public GpElementsPool<GpHttpClient::SP>
{
public:
    CLASS_REMOVE_CTRS(GpHttpClientPool)
    CLASS_DECLARE_DEFAULTS(GpHttpClientPool)
    CLASS_TAG(THREAD_SAFE)

public:
                                    GpHttpClientPool    (GpHttpClientFactory::SP aFactory) noexcept;
    virtual                         ~GpHttpClientPool   (void) noexcept override final;

protected:
    virtual void                    PreInit             (const count_t aCount) override final;
    virtual GpHttpClient::SP        NewElement          (void) override final;
    virtual void                    OnClear             (void) noexcept override final;
    virtual bool                    Validate            (GpHttpClient::SP aHttpClient) noexcept override final;

private:
    GpHttpClientFactory::SP         iFactory;
};

}//namespace GPlatform

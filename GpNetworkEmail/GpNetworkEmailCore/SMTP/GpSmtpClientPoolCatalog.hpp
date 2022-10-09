#pragma once

#include "GpSmtpClientPool.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpSmtpClientPoolCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSmtpClientPoolCatalog)
    CLASS_DD(GpSmtpClientPoolCatalog)
    CLASS_TAG(THREAD_SAFE)

    using PoolsT = GpElementsCatalog<std::string, GpSmtpClientPool::SP>;

private:
                                    GpSmtpClientPoolCatalog     (void) noexcept;
                                    ~GpSmtpClientPoolCatalog    (void) noexcept;

public:
    static GpSmtpClientPoolCatalog& S                           (void) noexcept;

    void                            Clear                       (void);

    void                            SetPoolDefaultFactory       (GpSmtpClientFactory::SP    aFactory,
                                                                 const size_t               aInitCount,
                                                                 const size_t               aMaxCount);

    void                            AddPool                     (GpSmtpClientPool::SP   aPool,
                                                                 std::string_view       aName);
    GpSmtpClientPool&               Pool                        (std::string_view aName);

private:
    PoolsT                          iPools;
    GpSmtpClientFactory::SP         iDefaultFactory;
    size_t                          iDefaultInitCount   = 0;
    size_t                          iDefaultMaxCount    = 0;
};

}//GPlatform

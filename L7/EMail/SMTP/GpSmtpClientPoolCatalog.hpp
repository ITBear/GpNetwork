#pragma once

#include "GpSmtpClientPool.hpp"

namespace GPlatform {

class GPNETWORK_API GpSmtpClientPoolCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSmtpClientPoolCatalog)
    CLASS_DECLARE_DEFAULTS(GpSmtpClientPoolCatalog)
    CLASS_TAG(THREAD_SAFE)

    using PoolsT = GpElementsCatalog<std::string, GpSmtpClientPool::SP>;

private:
                                    GpSmtpClientPoolCatalog     (void) noexcept;
                                    ~GpSmtpClientPoolCatalog    (void) noexcept;

public:
    static GpSmtpClientPoolCatalog& S                           (void) noexcept;

    void                            Clear                       (void);

    void                            SetPoolDefaultFactory       (GpSmtpClientFactory::SP    aFactory,
                                                                 const count_t              aInitCount,
                                                                 const count_t              aMaxCount);

    void                            AddPool                     (GpSmtpClientPool::SP   aPool,
                                                                 std::string_view       aName);
    GpSmtpClientPool&               Pool                        (std::string_view aName);

private:
    PoolsT                          iPools;
    GpSmtpClientFactory::SP         iDefaultFactory;
    count_t                         iDefaultInitCount   = 0_cnt;
    count_t                         iDefaultMaxCount    = 0_cnt;
};

}//GPlatform

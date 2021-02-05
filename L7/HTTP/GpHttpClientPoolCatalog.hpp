#pragma once

#include "GpHttpClientPool.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpClientPoolCatalog
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpClientPoolCatalog)
    CLASS_DECLARE_DEFAULTS(GpHttpClientPoolCatalog)
    CLASS_TAG(THREAD_SAFE)

    using PoolsT = GpElementsCatalog<std::string, GpHttpClientPool::SP, GpMap>;

private:
                                    GpHttpClientPoolCatalog     (void) noexcept;
                                    ~GpHttpClientPoolCatalog    (void) noexcept;

public:
    static GpHttpClientPoolCatalog& S                           (void) noexcept;

    void                            Clear                       (void);

    void                            SetPoolDefaultFactory       (GpHttpClientFactory::SP    aFactory,
                                                                 const count_t              aInitCount,
                                                                 const count_t              aMaxCount);

    void                            AddPool                     (GpHttpClientPool::SP   aPool,
                                                                 std::string_view       aName);
    GpHttpClientPool&               Pool                        (std::string_view aName);

private:
    PoolsT                          iPools;
    GpHttpClientFactory::SP         iDefaultFactory;
    count_t                         iDefaultInitCount   = 0_cnt;
    count_t                         iDefaultMaxCount    = 0_cnt;
};

}//GPlatform

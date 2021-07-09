#pragma once

#include "GpHttpClientPool.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpClientPoolCatalog
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClientPoolCatalog)
    CLASS_DECLARE_DEFAULTS(GpHttpClientPoolCatalog)
    CLASS_TAG(THREAD_SAFE)

    using PoolsT = GpElementsCatalog<std::string, GpHttpClientPool::SP>;

public:
                                    GpHttpClientPoolCatalog     (GpHttpClientFactory::SP    aFactory,
                                                                 const count_t              aMaxCalientCountPerPool) noexcept;
                                    ~GpHttpClientPoolCatalog    (void) noexcept;

    void                            Clear                       (void);
    GpHttpClientPool&               Pool                        (std::string_view aName);

private:
    GpHttpClientFactory::SP         iFactory;
    const count_t                   iMaxCalientCountPerPool;
    PoolsT                          iPools;
};

}//GPlatform

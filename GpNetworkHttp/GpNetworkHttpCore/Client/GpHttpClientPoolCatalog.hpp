#pragma once

#include "GpHttpClientPool.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpClientPoolCatalog
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClientPoolCatalog)
    CLASS_DD(GpHttpClientPoolCatalog)
    CLASS_TAG(THREAD_SAFE)

    using PoolsT = GpElementsCatalog<std::string, GpHttpClientPool::SP>;

public:
                                    GpHttpClientPoolCatalog     (GpHttpClientFactory::SP    aFactory,
                                                                 const size_t               aMaxClientsCountPerPool) noexcept;
                                    ~GpHttpClientPoolCatalog    (void) noexcept;

    void                            Clear                       (void);
    GpHttpClientPool&               Pool                        (std::string_view aName);

private:
    GpHttpClientFactory::SP         iFactory;
    const size_t                    iMaxClientsCountPerPool;
    PoolsT                          iPools;
};

}//GPlatform

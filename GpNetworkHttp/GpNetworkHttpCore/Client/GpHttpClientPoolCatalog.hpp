#pragma once

//#include "GpHttpClientPool.hpp"
//
//namespace GPlatform {
//
//class GP_NETWORK_HTTP_CORE_API GpHttpClientPoolCatalog
//{
//public:
//  CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClientPoolCatalog)
//  CLASS_DD(GpHttpClientPoolCatalog)
//  TAG_SET(THREAD_SAFE)
//
//  using PoolsT = GpDictionary<std::u8string, GpHttpClientPool::SP>;
//
//public:
//                                  GpHttpClientPoolCatalog     (GpHttpClientFactory::SP    aFactory,
//                                                               const size_t               aMaxClientsCountPerPool) noexcept;
//                                  ~GpHttpClientPoolCatalog    (void) noexcept;
//
//  void                            Clear                       (void);
//  GpHttpClientPool&               Pool                        (std::u8string_view aName);
//
//private:
//  GpHttpClientFactory::SP         iFactory;
//  const size_t                    iMaxClientsCountPerPool;
//  PoolsT                          iPools;
//};
//
//}//GPlatform

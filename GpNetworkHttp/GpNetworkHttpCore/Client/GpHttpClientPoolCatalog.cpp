#include "GpHttpClientPoolCatalog.hpp"

// namespace GPlatform {
//
//GpHttpClientPoolCatalog::GpHttpClientPoolCatalog
//(
//  GpHttpClientFactory::SP aFactory,
//  const size_t            aMaxClientsCountPerPool
//) noexcept:
//iFactory(std::move(aFactory)),
//iMaxClientsCountPerPool(aMaxClientsCountPerPool)
//{
//}
//
//GpHttpClientPoolCatalog::~GpHttpClientPoolCatalog (void) noexcept
//{
//}
//
//void  GpHttpClientPoolCatalog::Clear (void)
//{
//  iPools.Clear();
//}
//
//GpHttpClientPool& GpHttpClientPoolCatalog::Pool (std::string_view aName)
//{
//  return iPools.GetOrSet
//  (
//      aName,
//      [&]()
//      {
//          GpHttpClientPool::SP httpClientPool = MakeSP<GpHttpClientPool>(iFactory);
//          httpClientPool.V().Init(0, iMaxClientsCountPerPool);
//          return httpClientPool;
//      }
//  ).V();
//}
//
//}// namespace GPlatform

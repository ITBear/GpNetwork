#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Client/GpHttpClientPool.hpp>

// namespace GPlatform {
//
//GpHttpClientPool::GpHttpClientPool (GpHttpClientFactory::SP aFactory) noexcept:
//iFactory(std::move(aFactory))
//{
//}
//
//GpHttpClientPool::~GpHttpClientPool (void) noexcept
//{
//}
//
//GpHttpClient::SP  GpHttpClientPool::NewElement (GpSpinlock& /*aLocked*/)
//{
//  return iFactory.V().NewInstance();
//}
//
//bool  GpHttpClientPool::Validate (GpHttpClient::SP aHttpClient) noexcept
//{
//  if (aHttpClient.IsNULL())
//  {
//      return false;
//  }
//
//  return aHttpClient.Vn().IsValid();
//}
//
//}// namespace GPlatform

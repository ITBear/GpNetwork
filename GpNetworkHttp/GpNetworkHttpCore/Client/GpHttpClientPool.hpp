#pragma once

//#include "GpHttpClientFactory.hpp"
//#include "../../../../GpCore2/GpUtils/Types/Containers/GpElementsPool.hpp"
//
// namespace GPlatform {
//
//class GP_NETWORK_HTTP_CORE_API GpHttpClientPool final: public GpElementsPool<GpHttpClient::SP>
//{
//public:
//  CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClientPool)
//  CLASS_DD(GpHttpClientPool)
//  TAG_SET(THREAD_SAFE)
//
//public:
//                                  GpHttpClientPool    (GpHttpClientFactory::SP aFactory) noexcept;
//  virtual                         ~GpHttpClientPool   (void) noexcept override final;
//
//protected:
//  virtual void                    PreInit             (const size_t aCount) override final;
//  virtual GpHttpClient::SP        NewElement          (GpSpinlock& aLocked) override final;
//  virtual void                    OnClear             (void) noexcept override final;
//  virtual bool                    Validate            (GpHttpClient::SP aHttpClient) noexcept override final;
//
//private:
//  GpHttpClientFactory::SP         iFactory;
//};
//
//}// namespace GPlatform

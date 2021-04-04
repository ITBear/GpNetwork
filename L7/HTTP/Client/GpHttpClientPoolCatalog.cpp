#include "GpHttpClientPoolCatalog.hpp"

namespace GPlatform {

GpHttpClientPoolCatalog::GpHttpClientPoolCatalog
(
    GpHttpClientFactory::SP aFactory,
    const count_t           aMaxCalientCountPerPool
) noexcept:
iFactory(std::move(aFactory)),
iMaxCalientCountPerPool(aMaxCalientCountPerPool)
{
}

GpHttpClientPoolCatalog::~GpHttpClientPoolCatalog (void) noexcept
{
}

void    GpHttpClientPoolCatalog::Clear (void)
{
    iPools.Clear();
}

/*void  GpHttpClientPoolCatalog::SetPoolDefaultFactory
(
    GpHttpClientFactory::SP aFactory,
    const count_t           aInitCount,
    const count_t           aMaxCount
)
{
    iDefaultFactory     = aFactory;
    iDefaultInitCount   = aInitCount;
    iDefaultMaxCount    = aMaxCount;
}*/

/*void  GpHttpClientPoolCatalog::AddPool
(
    GpHttpClientPool::SP    aPool,
    std::string_view        aName
)
{
    ?
    iPools.Register(std::string(aName), std::move(aPool));
}*/

GpHttpClientPool&   GpHttpClientPoolCatalog::Pool (std::string_view aName)
{
    return iPools.FindOrRegister
    (
        aName,
        [&]()
        {
            GpHttpClientPool::SP httpClientPool = MakeSP<GpHttpClientPool>(iFactory);
            httpClientPool->Init(0_cnt, iMaxCalientCountPerPool);
            return httpClientPool;
        }
    ).V();

    /*if (res.has_value())
    {
        return res.value().get().V();
    } else if (iDefaultFactory.IsNotNULL())
    {
        GpHttpClientPool::SP newPool = MakeSP<GpHttpClientPool>(iDefaultFactory);
        newPool->Init(iDefaultInitCount, iDefaultMaxCount);
        AddPool(newPool, aName);
        return newPool.Vn();
    } else
    {
        THROW_GPE("Http client pool was not found by name '"_sv + aName + "'"_sv);
    }*/
}

}//namespace GPlatform

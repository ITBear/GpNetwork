#include "GpHttpClientPoolCatalog.hpp"

namespace GPlatform {

GpHttpClientPoolCatalog::GpHttpClientPoolCatalog (void) noexcept
{
}

GpHttpClientPoolCatalog::~GpHttpClientPoolCatalog (void) noexcept
{
    iPools.Clear();
}

GpHttpClientPoolCatalog&    GpHttpClientPoolCatalog::S (void) noexcept
{
    static GpHttpClientPoolCatalog sDbGlobal;
    return sDbGlobal;
}

void    GpHttpClientPoolCatalog::Clear (void)
{
    iPools.Clear();
}

void    GpHttpClientPoolCatalog::SetPoolDefaultFactory
(
    GpHttpClientFactory::SP aFactory,
    const count_t           aInitCount,
    const count_t           aMaxCount
)
{
    iDefaultFactory     = aFactory;
    iDefaultInitCount   = aInitCount;
    iDefaultMaxCount    = aMaxCount;
}

void    GpHttpClientPoolCatalog::AddPool
(
    GpHttpClientPool::SP    aPool,
    std::string_view        aName
)
{
    iPools.Register(std::string(aName), std::move(aPool));
}

GpHttpClientPool&   GpHttpClientPoolCatalog::Pool (std::string_view aName)
{
    auto res = iPools.Find(aName);

    if (res.has_value())
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
    }
}

}//namespace GPlatform

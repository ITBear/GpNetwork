#include "GpSmtpClientPoolCatalog.hpp"

namespace GPlatform {

GpSmtpClientPoolCatalog::GpSmtpClientPoolCatalog (void) noexcept
{
}

GpSmtpClientPoolCatalog::~GpSmtpClientPoolCatalog (void) noexcept
{
    iPools.Clear();
}

GpSmtpClientPoolCatalog&    GpSmtpClientPoolCatalog::S (void) noexcept
{
    static GpSmtpClientPoolCatalog sDbGlobal;
    return sDbGlobal;
}

void    GpSmtpClientPoolCatalog::Clear (void)
{
    iPools.Clear();
}

void    GpSmtpClientPoolCatalog::SetPoolDefaultFactory
(
    GpSmtpClientFactory::SP aFactory,
    const count_t           aInitCount,
    const count_t           aMaxCount
)
{
    iDefaultFactory     = aFactory;
    iDefaultInitCount   = aInitCount;
    iDefaultMaxCount    = aMaxCount;
}

void    GpSmtpClientPoolCatalog::AddPool
(
    GpSmtpClientPool::SP    aPool,
    std::string_view        aName
)
{
    iPools.Register(std::string(aName), std::move(aPool));
}

GpSmtpClientPool&   GpSmtpClientPoolCatalog::Pool (std::string_view aName)
{
    auto res = iPools.Find(aName);

    if (res.has_value())
    {
        return res.value().get().V();
    } else if (iDefaultFactory.IsNotNULL())
    {
        GpSmtpClientPool::SP newPool = MakeSP<GpSmtpClientPool>(iDefaultFactory);
        newPool->Init(iDefaultInitCount, iDefaultMaxCount);
        AddPool(newPool, aName);
        return newPool.Vn();
    } else
    {
        THROW_GPE("Smtp client pool was not found by name '"_sv + aName + "'"_sv);
    }
}

}//namespace GPlatform

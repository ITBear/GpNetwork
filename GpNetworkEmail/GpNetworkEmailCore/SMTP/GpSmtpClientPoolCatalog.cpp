#include "GpSmtpClientPoolCatalog.hpp"

namespace GPlatform {

GpSmtpClientPoolCatalog GpSmtpClientPoolCatalog::sInstance;

GpSmtpClientPoolCatalog::GpSmtpClientPoolCatalog (void) noexcept
{
}

GpSmtpClientPoolCatalog::~GpSmtpClientPoolCatalog (void) noexcept
{
    iPools.Clear();
}

void    GpSmtpClientPoolCatalog::Clear (void)
{
    iPools.Clear();
}

void    GpSmtpClientPoolCatalog::SetPoolDefaultFactory
(
    GpSmtpClientFactory::SP aFactory,
    const size_t            aInitCount,
    const size_t            aMaxCount
)
{
    iDefaultFactory     = aFactory;
    iDefaultInitCount   = aInitCount;
    iDefaultMaxCount    = aMaxCount;
}

void    GpSmtpClientPoolCatalog::AddPool
(
    GpSmtpClientPool::SP    aPool,
    std::u8string_view      aName
)
{
    iPools.Set(std::u8string(aName), std::move(aPool));
}

GpSmtpClientPool&   GpSmtpClientPoolCatalog::Pool (std::u8string_view aName)
{
    auto res = iPools.GetOpt(aName);

    if (res.has_value())
    {
        return res.value().get().V();
    } else if (iDefaultFactory.IsNotNULL())
    {
        GpSmtpClientPool::SP newPool = MakeSP<GpSmtpClientPool>(iDefaultFactory);
        newPool.V().Init(iDefaultInitCount, iDefaultMaxCount);
        AddPool(newPool, aName);
        return newPool.Vn();
    } else
    {
        THROW_GP(u8"Smtp client pool was not found by name '"_sv + aName + u8"'"_sv);
    }
}

}//namespace GPlatform

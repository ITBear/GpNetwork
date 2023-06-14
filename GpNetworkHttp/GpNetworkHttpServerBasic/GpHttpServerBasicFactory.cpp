#include "GpHttpServerBasicFactory.hpp"
#include "GpHttpServerBasic.hpp"

namespace GPlatform {

GpHttpServerBasicFactory::~GpHttpServerBasicFactory (void) noexcept
{
}

GpHttpServer::SP    GpHttpServerBasicFactory::NewInstance
(
    std::u8string       aName,
    GpIOEventPoller::SP aEventPoller
) const
{
    return MakeSP<GpHttpServerBasic>
    (
        std::move(aName),
        ListenSocketAddr(),
        std::move(aEventPoller),
        RequestHandlerFactory()
    );
}

}//namespace GPlatform

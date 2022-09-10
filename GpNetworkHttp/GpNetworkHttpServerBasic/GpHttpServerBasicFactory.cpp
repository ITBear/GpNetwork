#include "GpHttpServerBasicFactory.hpp"
#include "GpHttpServerBasic.hpp"

namespace GPlatform {

GpHttpServerBasicFactory::~GpHttpServerBasicFactory (void) noexcept
{
}

GpHttpServer::SP    GpHttpServerBasicFactory::NewInstance (std::string aName) const
{
    return MakeSP<GpHttpServerBasic>
    (
        std::move(aName),
        ListenSocketAddr(),
        iEventPoller,
        RequestHandlerFactory()
    );
}

}//namespace GPlatform

#include "GpHttpServerNodeFactory.hpp"
#include "GpHttpServerNode.hpp"

namespace GPlatform {

GpHttpServerNodeFactory::GpHttpServerNodeFactory
(
    const GpSocketAddr&             aListenSocketAddr,
    GpIOEventPoller::WP             aEventPoller,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
) noexcept:
GpHttpServerFactory(aListenSocketAddr,
                    std::move(aRequestHandlerFactory)),
iEventPoller(std::move(aEventPoller))
{
}

GpHttpServerNodeFactory::~GpHttpServerNodeFactory (void) noexcept
{
}

GpHttpServer::SP    GpHttpServerNodeFactory::NewInstance (std::string_view aName) const
{
    return MakeSP<GpHttpServerNode>
    (
        aName,
        ListenSocketAddr(),
        iEventPoller,
        RequestHandlerFactory()
    );
}

}//namespace GPlatform

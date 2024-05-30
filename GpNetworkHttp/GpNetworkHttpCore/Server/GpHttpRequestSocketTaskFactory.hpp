#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTaskFactory.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Routers/GpHttpRouter.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestSocketTaskFactory final: public GpSingleSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpRequestSocketTaskFactory)
    CLASS_DD(GpHttpRequestSocketTaskFactory)

public:
                                    GpHttpRequestSocketTaskFactory  (GpHttpRouter::SP   aRouter) noexcept;
    virtual                         ~GpHttpRequestSocketTaskFactory (void) noexcept override final;


    virtual GpSingleSocketTask::SP  NewInstance                     (GpSocket::SP       aSocket,
                                                                     GpIOEventPollerIdx aIOEventPollerIdx) const override final;

private:
    GpHttpRouter::SP                iRouter;
};

}// namespace GPlatform

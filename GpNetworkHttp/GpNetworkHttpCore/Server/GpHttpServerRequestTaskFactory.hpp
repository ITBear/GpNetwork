#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCore_global.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Routers/GpHttpRouter.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpTcpServerTaskFactory.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServerRequestTaskFactory final: public GpTcpServerTaskFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerRequestTaskFactory)
    CLASS_DD(GpHttpServerRequestTaskFactory)

public:
                                GpHttpServerRequestTaskFactory      (GpHttpRouter::SP   aRouter) noexcept;
    virtual                     ~GpHttpServerRequestTaskFactory (void) noexcept override final;


    virtual GpTcpServerTask::SP NewInstance                         (GpSocketTCP::SP    aSocketTCP,
                                                                     GpIOEventPollerIdx aIOEventPollerIdx) const override final;

private:
    GpHttpRouter::SP            iRouter;
};

}// namespace GPlatform

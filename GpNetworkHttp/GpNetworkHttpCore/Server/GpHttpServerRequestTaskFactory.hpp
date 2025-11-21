#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCore_global.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Routers/GpHttpRouter.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpSocketTaskFactory.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServerRequestTaskFactory final: public GpSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerRequestTaskFactory)
    CLASS_DD(GpHttpServerRequestTaskFactory)

public:
                                GpHttpServerRequestTaskFactory  (GpHttpRouter::SP   aRouter) noexcept;
    virtual                     ~GpHttpServerRequestTaskFactory (void) noexcept override final;


    virtual GpSocketTask::SP    NewInstance                     (GpSocket::UP aSocketUP) const override final;

private:
    GpHttpRouter::SP            iRouter;
};

}// namespace GPlatform

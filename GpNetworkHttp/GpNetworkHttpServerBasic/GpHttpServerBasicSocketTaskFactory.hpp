#pragma once

#include "GpNetworkHttpServerBasic_global.hpp"

namespace GPlatform {

class GpHttpServerBasicSocketTaskFactory final: public GpHttpServerSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpServerBasicSocketTaskFactory)
    CLASS_DD(GpHttpServerBasicSocketTaskFactory)

public:
                                GpHttpServerBasicSocketTaskFactory  (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept;
    virtual                     ~GpHttpServerBasicSocketTaskFactory (void) noexcept override final;

    virtual GpSocketTask::SP    NewInstance                         (std::string            aName,
                                                                     GpIOEventPoller::SP    aIOPoller,
                                                                     GpSocket::SP           aSocket) const override final;
};

}//namespace GPlatform

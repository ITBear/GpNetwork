#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../GpNetworkCore/Tasks/GpSingleSocketTaskFactory.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestSocketTaskFactory final: public GpSingleSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestSocketTaskFactory)
    CLASS_DD(GpHttpRequestSocketTaskFactory)

public:
                                    GpHttpRequestSocketTaskFactory  (void) noexcept = default;
    virtual                         ~GpHttpRequestSocketTaskFactory (void) noexcept override final;


    virtual GpSingleSocketTask::SP  NewInstance                     (GpSocket::SP aSocket) const override final;
};

}//namespace GPlatform

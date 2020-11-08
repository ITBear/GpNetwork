#pragma once

#include "../../IO/Sockets/GpSocketTaskFactory.hpp"

namespace GPlatform {

class GpHttpServerNodeSocketTaskFactory final: public GpSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpServerNodeSocketTaskFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerNodeSocketTaskFactory)

public:
                                GpHttpServerNodeSocketTaskFactory   (void) noexcept;
    virtual                     ~GpHttpServerNodeSocketTaskFactory  (void) noexcept override final;

    virtual GpSocketTask::SP    NewInstance                         (GpIOEventPoller::WP    aIOPooler,
                                                                     GpSocket::SP           aSocket) const override final;
};

}//namespace GPlatform

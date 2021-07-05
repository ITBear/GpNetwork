#pragma once

#include "../GpHttpServerSocketTaskFactory.hpp"

namespace GPlatform {

class GpHttpServerNodeSocketTaskFactory final: public GpHttpServerSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpServerNodeSocketTaskFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerNodeSocketTaskFactory)

public:
                                GpHttpServerNodeSocketTaskFactory   (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept;
    virtual                     ~GpHttpServerNodeSocketTaskFactory  (void) noexcept override final;

    virtual GpSocketTask::SP    NewInstance                         (std::string_view       aName,
                                                                     GpIOEventPoller::WP    aIOPoller,
                                                                     GpSocket::SP           aSocket) const override final;
};

}//namespace GPlatform

#pragma once

#include "../../IO/Sockets/GpSocketTaskFactory.hpp"
#include "GpHttpRequestHandlerFactory.hpp"

namespace GPlatform {

class GpHttpServerSocketTaskFactory: public GpSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS(GpHttpServerSocketTaskFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerSocketTaskFactory)

public:
                                    GpHttpServerSocketTaskFactory   (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept;
    virtual                         ~GpHttpServerSocketTaskFactory  (void) noexcept override;

    GpHttpRequestHandlerFactory::SP RequestHandlerFactory           (void) const {return iRequestHandlerFactory;}

private:
    GpHttpRequestHandlerFactory::SP iRequestHandlerFactory;
};

}//namespace GPlatform

#pragma once

#include "../RqRs/GpHttpRqRs.hpp"

namespace GPlatform {

class GpHttpServerSocketTaskFactory: public GpSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerSocketTaskFactory)
    CLASS_DECLARE_DEFAULTS(GpHttpServerSocketTaskFactory)

protected:
    inline                          GpHttpServerSocketTaskFactory   (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept;

public:
    virtual                         ~GpHttpServerSocketTaskFactory  (void) noexcept override {}

    GpHttpRequestHandlerFactory::SP RequestHandlerFactory           (void) const {return iRequestHandlerFactory;}

private:
    GpHttpRequestHandlerFactory::SP iRequestHandlerFactory;
};

GpHttpServerSocketTaskFactory::GpHttpServerSocketTaskFactory (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
}

}//namespace GPlatform

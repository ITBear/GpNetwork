#pragma once

#include "../GpHttpServer.hpp"
#include "GpHttpRouteHandlerPistache.hpp"

namespace GPlatform {

class GpHttpServerPistache final: public GpHttpServer
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpServerPistache)
    CLASS_DECLARE_DEFAULTS(GpHttpServerPistache)

public:
                            GpHttpServerPistache    (GpHttpRequestHandlerFactory::SP aRequestHandlerFactor) noexcept;
    virtual                 ~GpHttpServerPistache   (void) noexcept override final;

protected:
    virtual void            OnStart                 (void) override final;
    virtual GpTask::ResT    OnStep                  (EventOptRefT aEvent) override final;
    virtual void            OnStop                  (void) noexcept override final;

private:
    std::shared_ptr<Pistache::Http::Endpoint>       iHttpServer;
    Pistache::Rest::Router                          iHttpRouter;
    GpHttpRouteHandlerPistache::SP                  iHttpRouteHandler;
};

}//namespace GPlatform

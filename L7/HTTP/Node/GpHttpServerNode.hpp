#pragma once

#include "../GpHttpServer.hpp"
#include "../../../IO/Events/GpIOEventPoller.hpp"
#include "GpHttpServerNodeFactory.hpp"

namespace GPlatform {

class GpHttpServerNode final: public GpHttpServer
{
public:
    CLASS_REMOVE_CTRS(GpHttpServerNode)
    CLASS_DECLARE_DEFAULTS(GpHttpServerNode)

public:
                            GpHttpServerNode    (std::string_view                   aName,
                                                 const GpSocketAddr&                aListenSocketAddr,
                                                 GpIOEventPoller::WP                aEventPoller,
                                                 GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory);
    virtual                 ~GpHttpServerNode   (void) noexcept override final;

protected:
    virtual void            OnStart             (void) override final;
    virtual GpTask::ResT    OnStep              (EventOptRefT aEvent) override final;
    virtual void            OnStop              (void) noexcept override final;

private:
    void                    StartTcpServer      (std::string_view aTcpServerTaskName);

private:
    const GpSocketAddr      iListenSocketAddr;
    GpIOEventPoller::WP     iEventPoller;
};

}//namespace GPlatform

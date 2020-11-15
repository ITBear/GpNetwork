#pragma once

#include "../GpHttpServer.hpp"
#include "../../IO/Events/GpIOEventPoller.hpp"
#include "GpHttpServerNodeFactory.hpp"

namespace GPlatform {

class GpHttpServerNode final: public GpHttpServer
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpServerNode)
    CLASS_DECLARE_DEFAULTS(GpHttpServerNode)

public:
                                GpHttpServerNode    (const GpSocketAddr&                aListenSocketAddr,
                                                     GpIOEventPollerFactory::SP         aEventPollerFactory,
                                                     GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;
    virtual                     ~GpHttpServerNode   (void) noexcept override final;

protected:
    virtual void                OnStart             (void) override final;
    virtual GpTask::ResT        OnStep              (EventOptRefT aEvent) override final;
    virtual void                OnStop              (void) noexcept override final;

private:
    void                        StartIOPoller       (void);
    void                        StartTcpServer      (void);

private:
    const GpSocketAddr          iListenSocketAddr;
    GpIOEventPollerFactory::SP  iEventPollerFactory;    
    GpIOEventPoller::SP         iIOPoller;
};

}//namespace GPlatform

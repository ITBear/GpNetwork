#pragma once

#include "GpNetworkHttpServerBasic_global.hpp"

namespace GPlatform {

class GpHttpServerBasic final: public GpHttpServer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpServerBasic)
    CLASS_DD(GpHttpServerBasic)

public:
    inline                  GpHttpServerBasic   (std::string                        aName,
                                                 const GpSocketAddr&                aListenSocketAddr,
                                                 GpIOEventPoller::SP                aEventPoller,
                                                 GpHttpRequestHandlerFactory::SP    aRequestHandlerFactory) noexcept;
    virtual                 ~GpHttpServerBasic  (void) noexcept override final;

protected:
    virtual void            OnStart             (void) override final;
    virtual GpTaskDoRes     OnStep              (EventOptRefT aEvent) override final;
    virtual void            OnStop              (void) noexcept override final;

private:
    void                    StartTcpServer      (std::string aTcpServerTaskName);

private:
    const GpSocketAddr      iListenSocketAddr;
    GpIOEventPoller::SP     iEventPoller;
};

GpHttpServerBasic::GpHttpServerBasic
(
    std::string                     aName,
    const GpSocketAddr&             aListenSocketAddr,
    GpIOEventPoller::SP             aEventPoller,
    GpHttpRequestHandlerFactory::SP aRequestHandlerFactory
) noexcept:
GpHttpServer
(
    std::move(aName),
    std::move(aRequestHandlerFactory)
),
iListenSocketAddr(aListenSocketAddr),
iEventPoller(std::move(aEventPoller))
{
}

}//namespace GPlatform

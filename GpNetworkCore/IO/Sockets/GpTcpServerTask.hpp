#pragma once

#include "GpSocketTask.hpp"
#include "GpSocketTCP.hpp"
#include "GpSocketTaskFactory.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpTcpServerTask final: public GpSocketTask
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTcpServerTask)
    CLASS_DD(GpTcpServerTask)

public:
    inline                      GpTcpServerTask     (std::u8string              aName,
                                                     GpIOEventPoller::SP        aIOPoller,
                                                     GpSocketTaskFactory::SP    aTaskFactory,
                                                     GpSocketTCP::SP            aSocket) noexcept;
    virtual                     ~GpTcpServerTask    (void) noexcept override final;

    static  GpTcpServerTask::SP SConstruct          (std::u8string              aServerTaskName,
                                                     const GpSocketAddr&        aAddr,
                                                     const GpSocketFlags&       aFlags,
                                                     const size_t               aMaxListenQueueSize,
                                                     GpIOEventPoller::SP        aIOPoller,
                                                     GpSocketTaskFactory::SP    aTaskFactory);

protected:
    virtual GpTaskDoRes         OnSockReadyToRead   (GpSocket& aSocket) override final;
    virtual GpTaskDoRes         OnSockReadyToWrite  (GpSocket& aSocket) override final;
    virtual void                OnSockClosed        (GpSocket& aSocket) override final;
    virtual void                OnSockError         (GpSocket& aSocket) override final;

private:
    GpSocketTaskFactory::SP     iTaskFactory;
};

GpTcpServerTask::GpTcpServerTask
(
    std::u8string           aName,
    GpIOEventPoller::SP     aIOPoller,
    GpSocketTaskFactory::SP aTaskFactory,
    GpSocketTCP::SP         aSocket
) noexcept:
GpSocketTask
(
    std::move(aName),
    std::move(aIOPoller),
    std::move(aSocket)
),
iTaskFactory(std::move(aTaskFactory))
{
}

}//namespace GPlatform

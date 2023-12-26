#include "GpTcpAcceptServerTask.hpp"
#include "../Pollers/GpIOEventPollerCatalog.hpp"
#include "../Sockets/GpSocketTCP.hpp"

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <iostream>

namespace GPlatform {

class GpTcpAcceptServerTaskSocketFactory final: public GpSocketFactory
{
public:
    CLASS_DD(GpTcpAcceptServerTaskSocketFactory)

public:
                            GpTcpAcceptServerTaskSocketFactory  (GpSocketAddr           aAddr,
                                                                 const GpSocketFlags    aSocketFlags,
                                                                 const size_t           aMaxQueueSize,
                                                                 std::u8string          aEventPollerName) noexcept:
                                                                iAddr           (std::move(aAddr)),
                                                                iSocketFlags    (std::move(aSocketFlags)),
                                                                iMaxQueueSize   (aMaxQueueSize),
                                                                iEventPollerName(std::move(aEventPollerName)) {}
    virtual                 ~GpTcpAcceptServerTaskSocketFactory (void) noexcept override final = default;

    virtual GpSocket::SP    NewInstance                         (void) const override final
    {
        // Get event poller
        GpIOEventPoller::SP eventPoller = GpIOEventPollerCatalog::S().Get(iEventPollerName);

        // Create socket
        GpSocketTCP::SP socket = MakeSP<GpSocketTCP>
        (
            iSocketFlags | GpSocketFlag::NO_BLOCK,
            GpSocket::CloseModeT::CLOSE_ON_DESTRUCT
        );

        socket->Listen(iAddr, iMaxQueueSize);

        // Add soket to event poller
        const GpTaskId socketTaskId = GpTask::SCurrentTask().value().get().Id();
        eventPoller->AddSubscription
        (
            socket->Id(),
            socketTaskId,
            [](const GpTaskId aTaskId, const GpIOEventPoller::SubsriberResValT aIOEventsTypes)
            {
                GpTaskScheduler::S().MakeTaskReady(aTaskId, aIOEventsTypes);
            }
        );

        return socket;
    }

    virtual void            DestroyInstance     (GpSocket& aSocket) const override final
    {
        // Get event poller
        GpIOEventPoller::SP eventPoller = GpIOEventPollerCatalog::S().Get(iEventPollerName);

        // Remove socket from event poller
        const GpTaskId socketTaskId = GpTask::SCurrentTask().value().get().Id();
        eventPoller->RemoveSubscription(aSocket.Id(), socketTaskId);
    }

private:
    const GpSocketAddr  iAddr;
    const GpSocketFlags iSocketFlags;
    const size_t        iMaxQueueSize = 0;
    const std::u8string iEventPollerName;
};

GpTcpAcceptServerTask::GpTcpAcceptServerTask
(
    GpSocketAddr                    aSocketAddr,
    const size_t                    aSocketMaxQueueSize,
    const GpSocketFlags             aListenSocketFlags,
    const GpSocketFlags             aAcceptSocketFlags,
    std::u8string                   aEventPollerName,
    GpSingleSocketTaskFactory::SP   aTaskFactory
) noexcept:
GpSingleSocketTask
(
    MakeSP<GpTcpAcceptServerTaskSocketFactory>(aSocketAddr, aListenSocketFlags, aSocketMaxQueueSize, aEventPollerName)
),
iAcceptSocketFlags(aAcceptSocketFlags),
iTaskFactory      (std::move(aTaskFactory)),
iEventPollerName  (aEventPollerName)
{
}

GpTcpAcceptServerTask::~GpTcpAcceptServerTask (void) noexcept
{
}

void    GpTcpAcceptServerTask::OnReadyToRead (GpSocket& aSocket)
{
    GpSocketTCP& serverSocket = static_cast<GpSocketTCP&>(aSocket);

    if (iEventPoller == nullptr) [[unlikely]]
    {
        // Get event poller
        iEventPoller = GpIOEventPollerCatalog::S().Get(iEventPollerName).P();
    }

    // Accept
    size_t maxCount = 100;// max accepted sockets without YELD

    while (maxCount > 0)
    {
        maxCount--;

        GpSocketTCP::C::Opt::Val acceptedSocketOpt = serverSocket.Accept(iAcceptSocketFlags | GpSocketFlag::NO_BLOCK);

        if (!acceptedSocketOpt.has_value())
        {
            return;
        }

        GpSocketTCP&        acceptedSocket      = acceptedSocketOpt.value();
        const GpIOObjectId  acceptedSocketId    = acceptedSocket.Id();

        // Create socket task from factory
        GpSingleSocketTask::SP acceptedSocketTaskSP = iTaskFactory->NewInstance
        (
            MakeSP<GpSocketTCP>(std::move(acceptedSocket))
        );

        // Subscribe task to IO events
        const GpTaskId acceptedSocketTaskId = acceptedSocketTaskSP->Id();

        iEventPoller->AddSubscription
        (
            acceptedSocketId,
            acceptedSocketTaskId,
            [](const GpTaskId aTaskId, const GpIOEventPoller::SubsriberResValT aIOEventsTypes)
            {
                GpTaskScheduler::S().MakeTaskReady(aTaskId, aIOEventsTypes);
            }
        );

        // Add to scheduler
        GpTaskScheduler::S().NewToReady(std::move(acceptedSocketTaskSP));
    }

    return;
}

void    GpTcpAcceptServerTask::OnReadyToWrite (GpSocket& /*aSocket*/)
{
    //TODO: add message to log
}

void    GpTcpAcceptServerTask::OnClosed (GpSocket& /*aSocket*/)
{
    //TODO: add message to log
}

void    GpTcpAcceptServerTask::OnError (GpSocket& /*aSocket*/)
{
    //TODO: add message to log
}

}//namespace GPlatform

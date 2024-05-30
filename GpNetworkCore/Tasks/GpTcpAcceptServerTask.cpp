#include "GpTcpAcceptServerTask.hpp"
#include "../Pollers/GpIOEventPollerCatalog.hpp"
#include "../Sockets/GpSocketTCP.hpp"

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <iostream>

namespace GPlatform {

// --------------------------------------------- GpTcpAcceptServerTaskSocketFactory --------------------------------------------------

class GpTcpAcceptServerTaskSocketFactory final: public GpSocketFactory
{
public:
    CLASS_DD(GpTcpAcceptServerTaskSocketFactory)

public:
                                GpTcpAcceptServerTaskSocketFactory  (const GpSocketAddr         aAddr,
                                                                     const GpSocketFlags        aSocketFlags,
                                                                     const size_t               aMaxQueueSize,
                                                                     const GpIOEventPollerIdx   aIOEventPollerIdx) noexcept;
    virtual                     ~GpTcpAcceptServerTaskSocketFactory (void) noexcept override final;

    virtual GpSocket::SP        NewInstance                         (void) const override final;
    virtual void                OnStart                             (GpSocket::SP aSocket) const override final;
    virtual void                OnStop                              (GpSocket::SP aSocket) const override final;

private:
    const GpSocketAddr          iAddr;
    const GpSocketFlags         iSocketFlags;
    const size_t                iMaxQueueSize = 0;
    const GpIOEventPollerIdx    iIOEventPollerIdx;
};

GpTcpAcceptServerTaskSocketFactory::GpTcpAcceptServerTaskSocketFactory
(
    const GpSocketAddr          aAddr,
    const GpSocketFlags         aSocketFlags,
    const size_t                aMaxQueueSize,
    const GpIOEventPollerIdx    aIOEventPollerIdx
) noexcept:
iAddr            (aAddr),
iSocketFlags     (aSocketFlags),
iMaxQueueSize    (aMaxQueueSize),
iIOEventPollerIdx(aIOEventPollerIdx)
{
}

GpTcpAcceptServerTaskSocketFactory::~GpTcpAcceptServerTaskSocketFactory (void) noexcept
{
}


GpSocket::SP    GpTcpAcceptServerTaskSocketFactory::NewInstance (void) const
{
    // Create socket
    GpSocketTCP::SP socket = MakeSP<GpSocketTCP>
    (
        iSocketFlags | GpSocketFlag::NO_BLOCK,
        GpSocket::CloseModeT::CLOSE_ON_DESTRUCT
    );

    return socket;
}

void    GpTcpAcceptServerTaskSocketFactory::OnStart (GpSocket::SP aSocket) const
{
    GpSocketTCP& socketTcp = static_cast<GpSocketTCP&>(aSocket.Vn());

    // Listen socket
    socketTcp.Listen(iAddr, iMaxQueueSize);

    // Add socket to io poller subscription
    const bool isAdded = GpIOEventPollerCatalog::SAddSubscriptionSafe
    (
        socketTcp.Id(),
        GpTask::SCurrentTask().value().get().TaskId(),
        iIOEventPollerIdx
    );

    THROW_COND_GP
    (
        isAdded == true,
        "Failed to add subscription"
    );
}

void    GpTcpAcceptServerTaskSocketFactory::OnStop (GpSocket::SP aSocket) const
{
    GpSocketTCP& socketTcp = static_cast<GpSocketTCP&>(aSocket.Vn());

    // Remove socket from io poller subscription
    const bool isRemoved = GpIOEventPollerCatalog::SRemoveSubscriptionSafe
    (
        socketTcp.Id(),
        GpTask::SCurrentTask().value().get().TaskId(),
        iIOEventPollerIdx
    );

    THROW_COND_GP
    (
        isRemoved == true,
        "Failed to remove subscription"
    );
}

// --------------------------------------------- GpTcpAcceptServerTask --------------------------------------------------

GpTcpAcceptServerTask::GpTcpAcceptServerTask
(
    const GpSocketAddr              aSocketAddr,
    const size_t                    aSocketMaxQueueSize,
    const GpSocketFlags             aListenSocketFlags,
    const GpSocketFlags             aAcceptSocketFlags,
    const GpIOEventPollerIdx        aIOEventPollerIdx,
    GpSingleSocketTaskFactory::SP   aTaskFactory
) noexcept:
GpSingleSocketTask
(
    MakeSP<GpTcpAcceptServerTaskSocketFactory>(aSocketAddr, aListenSocketFlags, aSocketMaxQueueSize, aIOEventPollerIdx),
    aIOEventPollerIdx
),
iAcceptSocketFlags{aAcceptSocketFlags},
iTaskFactory      {std::move(aTaskFactory)}
{
}

GpTcpAcceptServerTask::~GpTcpAcceptServerTask (void) noexcept
{
}

void    GpTcpAcceptServerTask::OnReadyToRead (GpSocket& aSocket)
{
    GpSocketTCP& serverSocket = static_cast<GpSocketTCP&>(aSocket);

    // Accept
    size_t maxCount = 100;// TODO: move to config. Max accepted sockets without YELD

    while (maxCount > 0)
    {
        maxCount--;

        GpSocketTCP::C::Opt::Val acceptedSocketOpt = serverSocket.Accept(iAcceptSocketFlags | GpSocketFlag::NO_BLOCK);

        if (!acceptedSocketOpt.has_value())
        {
            break;
        }

        GpSocketTCP&        acceptedSocket      = acceptedSocketOpt.value();
        const GpSocketId    acceptedSocketId    = acceptedSocket.Id();

        // Create socket task from factory
        GpSingleSocketTask::SP socketTaskSP = iTaskFactory.Vn().NewInstance
        (
            MakeSP<GpSocketTCP>(std::move(acceptedSocket)),
            IOEventPollerIdx()
        );

        // Subscribe task to IO events
        const GpTaskId              acceptedSocketTaskId    = socketTaskSP.Vn().TaskId();
        const GpIOEventPollerIdx    ioEventPollerIdx        = IOEventPollerIdx();

        const bool isAdded = GpIOEventPollerCatalog::SAddSubscriptionSafe
        (
            acceptedSocketId,
            acceptedSocketTaskId,
            ioEventPollerIdx
        );

        THROW_COND_GP
        (
            isAdded == true,
            "Failed to add subscription"
        );

        // Add to scheduler
        GpTaskScheduler::S().NewToReady(std::move(socketTaskSP));
    }
}

void    GpTcpAcceptServerTask::OnReadyToWrite (GpSocket& /*aSocket*/)
{
    // TODO: add message to log
}

void    GpTcpAcceptServerTask::OnClosed (GpSocket& /*aSocket*/)
{
    // TODO: add message to log
}

void    GpTcpAcceptServerTask::OnError (GpSocket& /*aSocket*/)
{
    // TODO: add message to log
}

void    GpTcpAcceptServerTask::ProcessOtherMessages ([[maybe_unused]] GpAny& aMessage)
{
    // TODO: add message to log
}

}// namespace GPlatform

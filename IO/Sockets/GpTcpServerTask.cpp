#include "GpTcpServerTask.hpp"
#include <iostream>

namespace GPlatform {

GpTcpServerTask::GpTcpServerTask
(
    GpIOEventPoller::WP     aIOPooler,
    GpSocketTaskFactory::SP aTaskFactory,
    GpTaskScheduler::WP     aTaskScheduler,
    GpSocketTCP::SP         aSocket
) noexcept:
GpSocketTask(std::move(aIOPooler), std::move(aSocket)),
iTaskFactory(std::move(aTaskFactory)),
iTaskScheduler(std::move(aTaskScheduler))
{
}

GpTcpServerTask::~GpTcpServerTask (void) noexcept
{
}

GpTcpServerTask::SP GpTcpServerTask::SConstruct
(
    const GpSocketAddr&     aAddr,
    const GpSocketFlags&    aFlags,
    const count_t           aMaxListenQueueSize,
    GpIOEventPoller::SP     aIOPooler,
    GpSocketTaskFactory::SP aTaskFactory,
    GpTaskScheduler::WP     aTaskScheduler
)
{
    //Create socket
    GpSocketTCP::SP serverSocket = MakeSP<GpSocketTCP>(aFlags | GpSocketFlag::NO_BLOCK);
    serverSocket->Listen(aAddr, aMaxListenQueueSize);

    //Create server task
    GpTcpServerTask::SP serverSocketTask = MakeSP<GpTcpServerTask>
    (
        aIOPooler,
        std::move(aTaskFactory),
        std::move(aTaskScheduler),
        serverSocket
    );

    aIOPooler->AddSubscriber(serverSocketTask, serverSocket->Id());

    return serverSocketTask;
}

GpTask::ResT    GpTcpServerTask::OnSockReadyToRead (GpSocket& aSocket)
{
    GpSocketTCP& serverSocket = static_cast<GpSocketTCP&>(aSocket);

    //Accept
    GpSocket::SP inSocket;
    while ((inSocket = serverSocket.Accept(aSocket.Flags() | GpSocketFlag::NO_BLOCK)).IsNotNULL())
    {
        const GpSocketAddr::SocketIdT inSocketId = inSocket.VC().Id();

        //Start task
        GpSocketTask::SP inSocketTask = iTaskFactory.Vn().NewInstance(IOPooler(), std::move(inSocket));

        iTaskScheduler.V().AddTaskToWaiting(inSocketTask);
        IOPooler()->AddSubscriber(inSocketTask, inSocketId);
    }

    return GpTask::ResT::WAITING;
}

GpTask::ResT    GpTcpServerTask::OnSockReadyToWrite (GpSocket& /*aSocket*/)
{
    return GpTask::ResT::WAITING;
}

void    GpTcpServerTask::OnSockClosed (GpSocket& /*aSocket*/)
{
    //NOP
}

void    GpTcpServerTask::OnSockError (GpSocket& /*aSocket*/)
{
    //NOP
}

}//namespace GPlatform

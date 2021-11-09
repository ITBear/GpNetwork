#include "GpTcpServerTask.hpp"

namespace GPlatform {

GpTcpServerTask::GpTcpServerTask
(
    std::string_view        aName,
    GpIOEventPoller::WP     aIOPoller,
    GpSocketTaskFactory::SP aTaskFactory,
    GpTaskScheduler::WP     aTaskScheduler,
    GpSocketTCP::SP         aSocket
):
GpSocketTask(aName, std::move(aIOPoller), std::move(aSocket)),
iTaskFactory(std::move(aTaskFactory)),
iTaskScheduler(std::move(aTaskScheduler))
{
}

GpTcpServerTask::~GpTcpServerTask (void) noexcept
{
}

GpTcpServerTask::SP GpTcpServerTask::SConstruct
(
    std::string_view        aServerTaskName,
    const GpSocketAddr&     aAddr,
    const GpSocketFlags&    aFlags,
    const count_t           aMaxListenQueueSize,
    GpIOEventPoller::WP     aIOPoller,
    GpSocketTaskFactory::SP aTaskFactory,
    GpTaskScheduler::WP     aTaskScheduler
)
{
    //Create socket
    GpSocketTCP::SP serverSocket = MakeSP<GpSocketTCP>(aFlags | GpSocketFlag::NO_BLOCK, GpSocket::CloseModeT::CLOSE_ON_DESTRUCT);
    serverSocket->Listen(aAddr, aMaxListenQueueSize);

    //Create server task
    GpTcpServerTask::SP serverSocketTask = MakeSP<GpTcpServerTask>
    (
        aServerTaskName,
        aIOPoller,
        std::move(aTaskFactory),
        std::move(aTaskScheduler),
        serverSocket
    );

    aIOPoller->AddSubscriber(serverSocketTask, serverSocket->Id());

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
        GpSocketTask::SP inSocketTask = iTaskFactory.Vn().NewInstance
        (
            Name() + ": socket "_sv + inSocketId,
            IOPoller(),
            std::move(inSocket)
        );

        iTaskScheduler.V().AddTaskToWaiting(inSocketTask);
        IOPoller()->AddSubscriber(inSocketTask, inSocketId);
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

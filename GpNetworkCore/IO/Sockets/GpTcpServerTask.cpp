#include "GpTcpServerTask.hpp"

namespace GPlatform {

GpTcpServerTask::~GpTcpServerTask (void) noexcept
{
}

GpTcpServerTask::SP GpTcpServerTask::SConstruct
(
    std::u8string           aServerTaskName,
    const GpSocketAddr&     aAddr,
    const GpSocketFlags&    aFlags,
    const size_t            aMaxListenQueueSize,
    GpIOEventPoller::SP     aIOPoller,
    GpSocketTaskFactory::SP aTaskFactory
)
{
    //Create socket
    GpSocketTCP::SP serverSocket = MakeSP<GpSocketTCP>(aFlags | GpSocketFlag::NO_BLOCK, GpSocket::CloseModeT::CLOSE_ON_DESTRUCT);
    serverSocket.V().Listen(aAddr, aMaxListenQueueSize);

    //Create server task
    GpTcpServerTask::SP serverSocketTask = MakeSP<GpTcpServerTask>
    (
        std::move(aServerTaskName),
        aIOPoller,
        std::move(aTaskFactory),
        serverSocket
    );

    aIOPoller.V().AddSubscriber(serverSocketTask, serverSocket.V().Id());

    return serverSocketTask;
}

GpTaskDoRes GpTcpServerTask::OnSockReadyToRead (GpSocket& aSocket)
{
    GpSocketTCP& serverSocket = static_cast<GpSocketTCP&>(aSocket);

    //Accept
    GpSocket::SP inSocket;
    while ((inSocket = serverSocket.Accept(aSocket.Flags() | GpSocketFlag::NO_BLOCK)).IsNotNULL())
    {
        const GpSocketAddr::SocketIdT inSocketId = inSocket.V().Id();

        //Start task
        GpSocketTask::SP inSocketTask = iTaskFactory.Vn().NewInstance
        (
            Name() + u8": socket "_sv + inSocketId,
            IOPollerSP(),
            std::move(inSocket)
        );

        std::ignore = GpTaskScheduler::S().NewToWaitingDepend(inSocketTask);
        IOPoller().AddSubscriber(inSocketTask, inSocketId);
    }

    return GpTaskDoRes::WAITING;
}

GpTaskDoRes GpTcpServerTask::OnSockReadyToWrite (GpSocket& /*aSocket*/)
{
    return GpTaskDoRes::WAITING;
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

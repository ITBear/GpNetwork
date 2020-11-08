#include "GpTcpServerTask.hpp"
#include <iostream>

namespace GPlatform {

GpTcpServerTask::GpTcpServerTask (GpIOEventPoller::WP       aIOPooler,
                                  GpSocketTaskFactory::SP   aTaskFactory,
                                  GpTaskScheduler::WP       aTaskScheduler,
                                  GpSocketTCP::SP           aSocket) noexcept:
GpSocketTask(aIOPooler, std::move(aSocket)),
iTaskFactory(std::move(aTaskFactory)),
iTaskScheduler(std::move(aTaskScheduler))
{
}

GpTcpServerTask::~GpTcpServerTask (void) noexcept
{
}

GpTcpServerTask::SP GpTcpServerTask::SConstruct (const GpSocketAddr&        aAddr,
                                                 const GpSocketFlags&       aFlags,
                                                 const count_t              aMaxListenQueueSize,
                                                 GpIOEventPoller::SP        aIOPooler,
                                                 GpSocketTaskFactory::SP    aTaskFactory,
                                                 GpTaskScheduler::WP        aTaskScheduler)
{
    //Create socket
    GpSocketTCP::SP serverSocket = MakeSP<GpSocketTCP>(aFlags | GpSocketFlag::NO_BLOCK);
    serverSocket->Listen(aAddr, aMaxListenQueueSize);

    //Create server task
    GpTcpServerTask::SP serverSocketTask = MakeSP<GpTcpServerTask>(aIOPooler,
                                                                   aTaskFactory,
                                                                   aTaskScheduler,
                                                                   serverSocket);

    aIOPooler->AddSubscriber(serverSocketTask, serverSocket->Id());

    return serverSocketTask;
}

void    GpTcpServerTask::OnStart (void)
{
    GpSocketTask::OnStart();
}

GpTask::ResT    GpTcpServerTask::OnStep (EventOptRefT aEvent)
{
    return GpSocketTask::OnStep(aEvent);
}

void    GpTcpServerTask::OnStop (void) noexcept
{
    iTaskFactory.Clear();
    iTaskScheduler.Clear();

    GpSocketTask::OnStop();
}

GpTask::ResT    GpTcpServerTask::OnSockReadyToRead (GpSocket& aSocket)
{
    GpSocket::SP        inSocket        = static_cast<GpSocketTCP&>(aSocket).Accept(aSocket.Flags() | GpSocketFlag::NO_BLOCK);
    GpSocketTask::SP    inSocketTask    = iTaskFactory.Vn().NewInstance(IOPooler(), inSocket);

    iTaskScheduler.Vn().AddTaskToWaiting(inSocketTask);
    IOPooler()->AddSubscriber(inSocketTask, inSocket.VCn().Id());

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

#include "GpSocketTask.hpp"
#include "../Events/GpIOEvents.hpp"
#include <iostream>

namespace GPlatform {

GpSocketTask::GpSocketTask (GpIOEventPoller::WP aIOPooler,
                            GpSocket::SP        aSocket) noexcept:
iIOPooler(std::move(aIOPooler)),
iSocket(std::move(aSocket))
{
}

GpSocketTask::~GpSocketTask (void) noexcept
{
}

void    GpSocketTask::OnStart (void)
{
    THROW_GPE_COND_CHECK_M(iSocket.IsNotNULL(), "Socket is null"_sv);
}

GpTask::ResT    GpSocketTask::OnStep (EventOptRefT aEvent)
{
    if (!aEvent.has_value())
    {
        return ResT::WAITING;
    }

    const GpIOEvent&        ioEvent     = static_cast<const GpIOEvent&>(aEvent.value().get());
    const GpIOEventsTypes&  eventsMask  = ioEvent.Events();
    GpSocket&               socket      = iSocket.V();

    if (eventsMask.Test(GpIOEventType::READY_TO_READ))
    {
        std::cout << "[SocketTask::OnStep]: event READY_TO_READ"_sv << std::endl;
        if (OnSockReadyToRead(socket) == GpTask::ResT::DONE)
        {
            OnSockClosed(socket);
            OnStop();
            return GpTask::ResT::DONE;
        }
    }

    if (eventsMask.Test(GpIOEventType::READY_TO_WRITE))
    {
        std::cout << "[SocketTask::OnStep]: event READY_TO_WRITE"_sv << std::endl;
        if (OnSockReadyToWrite(socket) == GpTask::ResT::DONE)
        {
            OnSockClosed(socket);
            OnStop();
            return GpTask::ResT::DONE;
        }
    }

    if (eventsMask.Test(GpIOEventType::CLOSED))
    {
        std::cout << "[SocketTask::OnStep]: event CLOSED"_sv << std::endl;
        OnSockClosed(socket);
        OnStop();
        return GpTask::ResT::DONE;
    }

    if (eventsMask.Test(GpIOEventType::ERROR_OCCURRED))
    {
        std::cout << "[SocketTask::OnStep]: event ERROR_OCCURRED"_sv << std::endl;
        OnSockError(socket);
        OnStop();
        return GpTask::ResT::DONE;
    }

    return GpTask::ResT::WAITING;
}

void    GpSocketTask::OnStop (void) noexcept
{
    if (    iSocket.IsNotNULL()
         && iIOPooler.IsNotNULL())
    {
        GpSocket& sock = iSocket.Vn();
        GpSocketAddr::SocketIdT socketId = sock.Id();

        if (socketId != GpSocketAddr::sDefaultSocketId)
        {
            try
            {
                iIOPooler.Vn().RemoveSubscriber(socketId);
                sock.Close();
            } catch (const std::exception& e)
            {
                GpExceptionsSink::SSink(e);
            } catch (...)
            {
                GpExceptionsSink::SSinkUnknown();
            }           
        }

        iIOPooler.Clear();
        iSocket.Clear();
    }
}

}//namespace GPlatform

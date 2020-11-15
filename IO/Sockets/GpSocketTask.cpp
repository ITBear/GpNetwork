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

    //std::cout << "[GpSocketTask::OnStart]: " << this << ": " <<  sock.AddrLocal().ToString() << std::endl;
    //std::cout << "[GpSocketTask::OnStart]: " << this << ": " <<  sock.AddrRemote().ToString() << std::endl;
}

GpTask::ResT    GpSocketTask::OnStep (EventOptRefT aEvent)
{
    if (aEvent.has_value())
    {
        const GpEvent& event = aEvent.value().get();
        if (event.TypeInfo().UID() == GpIOEvent::STypeUID())
        {
            ProcessIOEvent(static_cast<const GpIOEvent&>(event));
        }
    }

    return GpTask::ResT::WAITING;
}

void    GpSocketTask::OnStop (void) noexcept
{
    if (   iSocket.IsNotNULL()
        && iIOPooler.IsNotNULL())
    {
        GpSocket& sock = iSocket.Vn();
        GpSocketAddr::SocketIdT socketId = sock.Id();

        if (socketId != GpSocketAddr::sDefaultSocketId)
        {
            try
            {
                iIOPooler.V().RemoveSubscriber(socketId);
                //TODO: add check on socket type (no need to close UDP, or server TCP)
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

void    GpSocketTask::ProcessIOEvent (const GpIOEvent& aIOEvent)
{
    const GpIOEventsTypes&  eventsMask  = aIOEvent.Events();
    GpSocket&               socket      = iSocket.V();

    if (eventsMask.Test(GpIOEventType::READY_TO_READ))
    {
        //std::cout << "[SocketTask::OnStep]: event READY_TO_READ: "_sv << iSocket->Id() << std::endl;
        if (OnSockReadyToRead(socket) == GpTask::ResT::DONE)
        {
            OnSockClosed(socket);
            GpTaskFiberCtx::SYeld(GpTask::ResT::DONE);
        }
    }

    if (eventsMask.Test(GpIOEventType::READY_TO_WRITE))
    {
        //std::cout << "[SocketTask::OnStep]: event READY_TO_WRITE: "_sv << iSocket->Id() << std::endl;
        if (OnSockReadyToWrite(socket) == GpTask::ResT::DONE)
        {
            OnSockClosed(socket);
            GpTaskFiberCtx::SYeld(GpTask::ResT::DONE);
        }
    }

    if (eventsMask.Test(GpIOEventType::CLOSED))
    {
        //std::cout << "[SocketTask::OnStep]: event CLOSED: "_sv << iSocket->Id() << std::endl;
        OnSockClosed(socket);
        GpTaskFiberCtx::SYeld(GpTask::ResT::DONE);
    }

    if (eventsMask.Test(GpIOEventType::ERROR_OCCURRED))
    {
        //std::cout << "[SocketTask::OnStep]: event ERROR_OCCURRED: "_sv << iSocket->Id() << std::endl;
        OnSockError(socket);
        GpTaskFiberCtx::SYeld(GpTask::ResT::DONE);
    }
}

}//namespace GPlatform

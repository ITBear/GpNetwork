#include "GpSocketTask.hpp"
#include "../Events/GpIOEvents.hpp"

#include <iostream>

namespace GPlatform {

static int _GpSocketTask_count = 0;

GpSocketTask::GpSocketTask
(
    std::string_view    aName,
    GpIOEventPoller::WP aIOPoller,
    GpSocket::SP        aSocket
):
GpTaskFiberBase(aName),
iIOPoller(std::move(aIOPoller)),
iSocket(std::move(aSocket))
{
    _GpSocketTask_count++;
    std::cout << "[GpSocketTask::GpSocketTask]: count = " << _GpSocketTask_count << std::endl;
}

GpSocketTask::~GpSocketTask (void) noexcept
{
    _GpSocketTask_count--;
    std::cout << "[GpSocketTask::~GpSocketTask]: count = " << _GpSocketTask_count << std::endl;
}

void    GpSocketTask::OnStart (void)
{   
    THROW_GPE_COND(iSocket.IsNotNULL(), "Socket is null"_sv);
}

GpTask::ResT    GpSocketTask::OnStep (EventOptRefT aEvent)
{
    if (aEvent.has_value())
    {
        const GpEvent& event = aEvent.value().get();
        if (event.TypeInfo().UID() == GpIOEvent::STypeUID())
        {
            if (ProcessIOEvent(static_cast<const GpIOEvent&>(event)) == GpTask::ResT::DONE)
            {
                OnSockClosed(iSocket.V());
                return GpTask::ResT::DONE;
            }
        }
    }

    return GpTask::ResT::WAITING;
}

void    GpSocketTask::OnStop (void) noexcept
{
    if (iSocket.IsNotNULL())
    {
        GpSocket&               sock        = iSocket.Vn();
        GpSocketAddr::SocketIdT socketId    = sock.Id();

        if (iIOPoller.IsNotNULL())
        {
            if (socketId != GpSocketAddr::sDefaultSocketId)
            {
                try
                {
                    iIOPoller.V().RemoveSubscriber(socketId);
                } catch (const std::exception& e)
                {
                    GpExceptionsSink::SSink(e);
                } catch (...)
                {
                    GpExceptionsSink::SSinkUnknown();
                }

                try
                {
                    if (sock.CloseMode() == GpSocket::CloseModeT::CLOSE_ON_DESTRUCT)
                    {
                        sock.Close();
                    }
                } catch (const std::exception& e)
                {
                    GpExceptionsSink::SSink(e);
                } catch (...)
                {
                    GpExceptionsSink::SSinkUnknown();
                }
            }

            iIOPoller.Clear();
        }

        iSocket.Clear();
    }
}

GpTask::ResT    GpSocketTask::ProcessIOEvent (const GpIOEvent& aIOEvent)
{
    const GpIOEventsTypes&  eventsMask  = aIOEvent.Events();
    GpSocket&               socket      = iSocket.V();

    if (eventsMask.Test(GpIOEventType::READY_TO_READ))
    {
        if (OnSockReadyToRead(socket) == GpTask::ResT::DONE)
        {
            return GpTask::ResT::DONE;
        }
    }

    if (eventsMask.Test(GpIOEventType::READY_TO_WRITE))
    {
        if (OnSockReadyToWrite(socket) == GpTask::ResT::DONE)
        {
            return GpTask::ResT::DONE;
        }
    }

    if (eventsMask.Test(GpIOEventType::CLOSED))
    {
        return GpTask::ResT::DONE;
    }

    if (eventsMask.Test(GpIOEventType::ERROR_OCCURRED))
    {
        OnSockError(socket);
        return GpTask::ResT::DONE;
    }

    return GpTask::ResT::WAITING;
}

}//namespace GPlatform

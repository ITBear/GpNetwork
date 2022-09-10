#include "GpSocketTask.hpp"
#include "../Events/GpIOEvents.hpp"

namespace GPlatform {

GpSocketTask::~GpSocketTask (void) noexcept
{
}

void    GpSocketTask::OnStart (void)
{
    THROW_COND_GP(iSocket.IsNotNULL(), "Socket is null"_sv);
}

GpTaskDoRes GpSocketTask::OnStep (EventOptRefT aEvent)
{
    if (aEvent.has_value())
    {
        const GpEvent& event = aEvent.value().get();

        if (event.Type() == GpIOEvent::SType())
        {
            const GpIOEvent& ioEvent = static_cast<const GpIOEvent&>(event);
            if (ProcessIOEvent(ioEvent) == GpTaskDoRes::DONE)
            {
                OnSockClosed(iSocket.V());
                return GpTaskDoRes::DONE;
            }
        }
    }

    return GpTaskDoRes::WAITING;
}

void    GpSocketTask::OnStop (void) noexcept
{
    if (iSocket.IsNotNULL())
    {
        GpSocket&               sock        = iSocket.Vn();
        GpSocketAddr::SocketIdT socketId    = sock.Id();

        if (socketId != GpSocketAddr::sDefaultSocketId)
        {
            try
            {
                iIOPoller.RemoveSubscriber(socketId);
            } catch (const GpException& e)
            {
                LOG_EXCEPTION(e, Guid());
            } catch (const std::exception& e)
            {
                LOG_EXCEPTION(e, Guid());
            } catch (...)
            {
                LOG_EXCEPTION(Guid());
            }

            try
            {
                if (sock.CloseMode() == GpSocket::CloseModeT::CLOSE_ON_DESTRUCT)
                {
                    sock.Close();
                }
            } catch (const GpException& e)
            {
                LOG_EXCEPTION(e, Guid());
            } catch (const std::exception& e)
            {
                LOG_EXCEPTION(e, Guid());
            } catch (...)
            {
                LOG_EXCEPTION(Guid());
            }
        }

        iSocket.Clear();
    }
}

GpTaskDoRes GpSocketTask::ProcessIOEvent (const GpIOEvent& aIOEvent)
{
    const GpIOEventsTypes&  eventsMask  = aIOEvent.Events();
    GpSocket&               socket      = iSocket.V();

    if (eventsMask.Test(GpIOEventType::READY_TO_READ))
    {
        if (OnSockReadyToRead(socket) == GpTaskDoRes::DONE)
        {
            return GpTaskDoRes::DONE;
        }
    }

    if (eventsMask.Test(GpIOEventType::READY_TO_WRITE))
    {
        if (OnSockReadyToWrite(socket) == GpTaskDoRes::DONE)
        {
            return GpTaskDoRes::DONE;
        }
    }

    if (eventsMask.Test(GpIOEventType::CLOSED))
    {
        return GpTaskDoRes::DONE;
    }

    if (eventsMask.Test(GpIOEventType::ERROR_OCCURRED))
    {
        OnSockError(socket);
        return GpTaskDoRes::DONE;
    }

    return GpTaskDoRes::WAITING;
}

}//namespace GPlatform

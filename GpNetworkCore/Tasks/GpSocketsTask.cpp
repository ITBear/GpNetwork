#include "GpSocketsTask.hpp"

#include "../Pollers/GpIOEventPoller.hpp"

namespace GPlatform {

GpSocketsTask::~GpSocketsTask (void) noexcept
{
}

void    GpSocketsTask::OnStart (void)
{
}

GpTaskRunRes::EnumT GpSocketsTask::OnStep (void)
{
    GpAny::C::Opt::Val messageOpt = PopMessage();

    while (   (!IsStopCalled())
           && (messageOpt.has_value()))
    {
        auto& message = messageOpt.value();
        if (message.IsContatinType<GpIOEventPoller::SubsriberResValT>()) [[likely]]
        {
            const GpIOEventPoller::SubsriberResValT& pollerEvents = message.ValueNoCheck<GpIOEventPoller::SubsriberResValT>();
            ProcessSocketEvents(std::get<0>(pollerEvents), std::get<1>(pollerEvents));
        } else
        {
            ProcessOtherMessages(message);
        }

        messageOpt = PopMessage();
    }

    return GpTaskRunRes::WAIT;
}

GpException::C::Opt GpSocketsTask::OnStop (void) noexcept
{
    return std::nullopt;
}

void    GpSocketsTask::ProcessSocketEvents
(
    const GpSocketId        aSocketId,
    const GpIOEventsTypes   aIoEvents
)
{
    GpSocket::SP socketSP = FindSocket(aSocketId);

    if (socketSP.IsNULL()) [[unlikely]]
    {
        return;
    }

    GpSocket& socket = socketSP.Vn();

    if (aIoEvents.Test(GpIOEventType::READY_TO_READ))
    {
        OnReadyToRead(socket);
    }

    if (aIoEvents.Test(GpIOEventType::READY_TO_WRITE))
    {
        OnReadyToWrite(socket);
    }

    if (aIoEvents.Test(GpIOEventType::CLOSED)) [[unlikely]]
    {
        OnClosed(socket);
    } else if (aIoEvents.Test(GpIOEventType::ERROR_OCCURRED)) [[unlikely]]
    {
        OnError(socket);
    }
}

}// namespace GPlatform

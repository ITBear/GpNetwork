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
    GpTaskPayloadStorage::AnyOptT payloadOpt = PopPayload();

    while (payloadOpt.has_value())
    {
        auto& payload = payloadOpt.value();
        if (payload.IsContatinType<GpIOEventPoller::SubsriberResValT>()) [[likely]]
        {
            const GpIOEventPoller::SubsriberResValT& pollerEvents = payload.ValueNoCheck<GpIOEventPoller::SubsriberResValT>();
            ProcessSocketEvents(std::get<0>(pollerEvents), std::get<1>(pollerEvents));
        } else
        {
            ProcessOtherEvents(payload);
        }

        payloadOpt = PopPayload();
    }

    return GpTaskRunRes::WAIT;
}

std::optional<GpException>  GpSocketsTask::OnStop (void) noexcept
{
    return std::nullopt;
}

void    GpSocketsTask::ProcessOtherEvents (GpAny& /*aEvent*/)
{
    // NOP
}

void    GpSocketsTask::ProcessSocketEvents
(
    const GpIOObjectId      aSocketId,
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
    } else if (aIoEvents.Test(GpIOEventType::ERROR)) [[unlikely]]
    {
        OnError(socket);
    }
}

}// namespace GPlatform

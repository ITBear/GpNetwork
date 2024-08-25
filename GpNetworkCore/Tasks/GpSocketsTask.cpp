#include <GpNetwork/GpNetworkCore/Tasks/GpSocketsTask.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp>
#include <GpLog/GpLogCore/GpLog.hpp>

namespace GPlatform {

GpSocketsTask::GpSocketsTask (void) noexcept
{
}

GpSocketsTask::GpSocketsTask (std::string aTaskName) noexcept:
GpTaskFiber{std::move(aTaskName)}
{
}

GpSocketsTask::~GpSocketsTask (void) noexcept
{
}

void    GpSocketsTask::OnStart (void)
{
    // NOP
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
            if (!message.Empty())
            {
                ProcessOtherMessages(message);
            }
        }

        messageOpt = PopMessage();
    }

    return GpTaskRunRes::WAIT;
}

void    GpSocketsTask::OnStop (StopExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        LOG_INFO("[GpSocketsTask::OnStop]: ..."_sv);
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& ex)
    {
        aStopExceptionsOut.emplace_back(GpException{ex.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpSocketsTask::OnStop]: unknown exception"_sv});
    }
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

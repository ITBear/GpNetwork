#include "GpSocketTask.hpp"

namespace GPlatform {

GpSocketTask::~GpSocketTask (void) noexcept
{
}

void    GpSocketTask::OnStart (void)
{
    if (iSocket.IsNULL())
    {
        if (iSocketFactory.IsNULL())
        {
            THROW_GP(u8"Socket is null and socker factory is null"_sv);
        }

        iSocket = iSocketFactory->NewInstance();
    }

    if (!iSocket->IsValidId())
    {
        THROW_GP(u8"Socket is invalid"_sv);
    }
}

GpTaskRunRes::EnumT GpSocketTask::OnStep (void)
{
    GpTaskPayloadStorage::AnyOptT payload = PopPayload();

    if (!payload.has_value())
    {
        return GpTaskRunRes::WAIT;
    }

    const GpIOEventsTypes ioEvents = payload.value().Value<GpIOEventsTypes>();
    if (ProcessEvents(ioEvents) == GpTaskRunRes::DONE)
    {
        return GpTaskRunRes::DONE;
    }

    return GpTaskRunRes::WAIT;
}

std::optional<GpException>  GpSocketTask::OnStop (void) noexcept
{
    std::optional<GpException> ex;

    try
    {
        if (iSocket.IsNotNULL())
        {
            OnClosed(iSocket.Vn());
            iSocket->Close();
            iSocket.Clear();
        }

    } catch (const GpException& e)
    {
        ex = e;
    } catch (const std::exception& e)
    {
        ex = GpException(e.what());
    } catch (...)
    {
        ex = GpException(u8"[GpSocketTask::OnStop]: unknown exception"_sv);
    }

    return ex;
}

GpTaskRunRes::EnumT GpSocketTask::ProcessEvents (const GpIOEventsTypes aIoEvents)
{
    GpSocket&           socket  = iSocket.V();
    GpTaskRunRes::EnumT res     = GpTaskRunRes::WAIT;

    if (aIoEvents.Test(GpIOEventType::READY_TO_READ))
    {
        const GpTaskRunRes::EnumT localRes = OnReadyToRead(socket);
        res = std::max(res, localRes);
    }

    if (aIoEvents.Test(GpIOEventType::READY_TO_WRITE))
    {
        const GpTaskRunRes::EnumT localRes = OnReadyToWrite(socket);
        res = std::max(res, localRes);
    }

    if (aIoEvents.Test(GpIOEventType::CLOSED))
    {
        OnClosed(socket);
        res = GpTaskRunRes::DONE;
    }

    if (aIoEvents.Test(GpIOEventType::ERROR))
    {
        OnError(socket);
        res = GpTaskRunRes::DONE;
    }

    return res;
}

}//namespace GPlatform

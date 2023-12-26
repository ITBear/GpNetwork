#include "GpSingleSocketTask.hpp"

namespace GPlatform {

GpSingleSocketTask::~GpSingleSocketTask (void) noexcept
{
}

void    GpSingleSocketTask::OnStart (void)
{
    GpSocketsTask::OnStart();

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

std::optional<GpException>  GpSingleSocketTask::OnStop (void) noexcept
{
    std::optional<GpException> ex;

    try
    {
        if (iSocket.IsNotNULL())
        {
            if (iSocketFactory.IsNotNULL())
            {
                iSocketFactory.Vn().DestroyInstance(iSocket.Vn());
            }

            iSocket->Close();
            OnClosed(iSocket.Vn());
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
        ex = GpException(u8"[GpSingleSocketTask::OnStop]: unknown exception"_sv);
    }

    std::optional<GpException> inhEx = GpSocketsTask::OnStop();

    return ex.has_value() ? ex : inhEx;
}

GpSocket::SP    GpSingleSocketTask::FindSocket ([[maybe_unused]] const GpIOObjectId aSocketId)
{
    if (iSocket.IsNotNULL()) [[likely]]
    {
        return iSocket;
    } else
    {
        return GpSocket::SP::SNull();
    }
}

}// namespace GPlatform

#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTask.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

GpSingleSocketTask::GpSingleSocketTask
(
    GpSocketFactory::SP         aSocketFactory,
    const GpIOEventPollerIdx    aIOEventPollerIdx
) noexcept:
iSocketFactory   {std::move(aSocketFactory)},
iIOEventPollerIdx{std::move(aIOEventPollerIdx)}
{
}

GpSingleSocketTask::GpSingleSocketTask
(
    GpSocketFactory::SP         aSocketFactory,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    std::string                 aTaskName
) noexcept:
GpSocketsTask{std::move(aTaskName)},
iSocketFactory   {std::move(aSocketFactory)},
iIOEventPollerIdx{std::move(aIOEventPollerIdx)}
{
}

GpSingleSocketTask::GpSingleSocketTask
(
    GpSocket::SP                aSocket,    
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    std::string                 aTaskName
) noexcept:
GpSocketsTask{std::move(aTaskName)},
iSocket          {std::move(aSocket)},
iIOEventPollerIdx{std::move(aIOEventPollerIdx)}
{
}

GpSingleSocketTask::GpSingleSocketTask
(
    GpSocket::SP                aSocket,
    const GpIOEventPollerIdx    aIOEventPollerIdx
) noexcept:
iSocket          {std::move(aSocket)},
iIOEventPollerIdx{std::move(aIOEventPollerIdx)}
{
}

GpSingleSocketTask::~GpSingleSocketTask (void) noexcept
{
}

void    GpSingleSocketTask::OnStart (void)
{
    GpSocketsTask::OnStart();

    if (iSocket.IsNULL())
    {
        THROW_COND_GP
        (
            iSocketFactory.IsNotNULL(),
            "Socket is null and socket factory is null"
        );

        iSocket = iSocketFactory->NewInstance();
        iSocketFactory->OnStart(iSocket);
    }

    if (!iSocket->IsValidId())
    {
        THROW_GP("Socket is invalid");
    }
}

GpException::C::Opt GpSingleSocketTask::OnStop (void) noexcept
{
    GpException::C::Opt ex;

    try
    {
        if (iSocket.IsNotNULL())
        {
            GpSocket& socket = iSocket.Vn();

            if (iSocketFactory.IsNotNULL())
            {
                iSocketFactory->OnStop(iSocket);
            }

            socket.Close();
            OnClosed(socket);
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
        ex = GpException("[GpSingleSocketTask::OnStop]: unknown exception"_sv);
    }

    GpException::C::Opt inhEx = GpSocketsTask::OnStop();

    return ex.has_value() ? ex : inhEx;
}

GpSocket::SP    GpSingleSocketTask::FindSocket ([[maybe_unused]] const GpSocketId aSocketId)
{
    return iSocket;
}

}// namespace GPlatform

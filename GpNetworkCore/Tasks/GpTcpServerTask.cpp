#include <GpNetwork/GpNetworkCore/Tasks/GpTcpServerTask.hpp>

namespace GPlatform {

GpTcpServerTask::GpTcpServerTask (GpSocketTCP::UP aSocketTcpUP) noexcept:
GpSingleSocketTask{std::move(aSocketTcpUP)}
{
}

GpTcpServerTask::GpTcpServerTask
(
    GpSocketTCP::UP aSocketTcpUP,
    std::string     aTaskName
) noexcept:
GpSingleSocketTask{std::move(aSocketTcpUP), std::move(aTaskName)}
{
}

GpTcpServerTask::~GpTcpServerTask (void) noexcept
{
}

void    GpTcpServerTask::OnStop (ExceptionsT& aStopExceptionsOut) noexcept
{
    try
    {
        GpUniqueLock uniqueLock{SpinLock()};
        Socket().Close();
    } catch (const GpException& ex)
    {
        aStopExceptionsOut.emplace_back(ex);
    } catch (const std::exception& ex)
    {
        aStopExceptionsOut.emplace_back(GpException{ex.what()});
    } catch (...)
    {
        aStopExceptionsOut.emplace_back(GpException{"[GpTcpServerTask::OnStop]: unknown exception"_sv});
    }
}

}// namespace GPlatform

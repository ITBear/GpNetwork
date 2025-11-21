#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTask.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpCore2/GpUtils/Threads/Timers/GpTimersManager.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpTcpServerTask: public GpSingleSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTcpServerTask)
    CLASS_DD(GpTcpServerTask)

protected:
                    GpTcpServerTask     (GpSocketTCP::UP    aSocketTcpUP) noexcept;
                    GpTcpServerTask     (GpSocketTCP::UP    aSocketTcpUP,
                                         std::string        aTaskName) noexcept;

protected:
    virtual void    OnStop              (ExceptionsT& aStopExceptionsOut) noexcept override;

public:
    virtual         ~GpTcpServerTask    (void) noexcept override;
};

}// namespace GPlatform

#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTask.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpCore2/GpUtils/Threads/Timers/GpTimersManager.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpTcpClientTask: public GpSingleSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTcpClientTask)
    CLASS_DD(GpTcpClientTask)

protected:
                        GpTcpClientTask     (GpSocketTCP::UP    aSocketUP) noexcept;
                        GpTcpClientTask     (GpSocketTCP::UP    aSocketUP,
                                             std::string        aTaskName) noexcept;

public:
    virtual             ~GpTcpClientTask    (void) noexcept override;

protected:
    // Call from OnStart
    GpSocketStateTCP    ConnectTo           (std::string_view   aDomainName,
                                             u_int_16           aPort,
                                             GpSocketIPv        aIPv,
                                             milliseconds_t     aConnectTimeout,
                                             GpTask::WP         aTaskWP);
    // Call from OnStart
    GpSocketStateTCP    ConnectTo           (const GpSocketAddr&    aServerAddr,
                                             milliseconds_t         aConnectTimeout,
                                             GpTask::WP             aTaskWP);

    virtual void        OnStart             (void) override = 0;
    virtual void        OnStop              (ExceptionsT& aStopExceptionsOut) noexcept override;
    virtual void        OnStopException     (const GpException& aException) noexcept override = 0;

    virtual void        OnReadyToRead       (GpSocket& aSocket) REQUIRES(SpinLock()) override = 0;
    virtual void        OnReadyToWrite      (GpSocket& aSocket) REQUIRES(SpinLock()) override = 0;
    virtual void        OnClosed            (GpSocket& aSocket) REQUIRES(SpinLock()) override = 0;
    virtual void        OnError             (GpSocket& aSocket) REQUIRES(SpinLock()) override = 0;

    virtual void        OnConnected         (GpSocketTCP& aSocket) REQUIRES(SpinLock()) = 0;
    virtual void        OnConnectionTimeout (GpSocketTCP& aSocket) REQUIRES(SpinLock()) = 0;

private:
    virtual void        ProcessSocketEvents (GpSocket&          aSocket,
                                             GpIOEventsTypes    aIoEvents) REQUIRES(SpinLock()) override final;
    bool                VerifyConnectTo     (const GpSocketAddr& aServerAddr) const REQUIRES(SpinLock());

private:
    bool    iIsConnected GUARDED_BY(SpinLock()) = false;
};

}// namespace GPlatform

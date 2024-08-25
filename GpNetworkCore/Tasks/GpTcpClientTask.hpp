#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSocketsTask.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_CORE_API, GpTcpClientConnectionState,
    NOT_CONNECTED,
    CONNECTION_IN_PROGRESS,
    CONNECTED
);

class GP_NETWORK_CORE_API GpTcpClientTask: public GpSocketsTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTcpClientTask)
    CLASS_DD(GpTcpClientTask)

    using ConnectionStateT  = GpTcpClientConnectionState;
    using ConnectionStateTE = ConnectionStateT::EnumT;

protected:
                                GpTcpClientTask             (GpSocketFlags      aSocketFlags,
                                                             GpIOEventPollerIdx aIOEventPollerIdx);
                                GpTcpClientTask             (GpSocketFlags      aSocketFlags,
                                                             GpIOEventPollerIdx aIOEventPollerIdx,
                                                             std::string        aTaskName);

public:
    virtual                     ~GpTcpClientTask            (void) noexcept override;

    void                        CloseTcpConnection          (void);
    void                        ConnectTcpAndWait           (std::string_view   aDomainName,
                                                             u_int_16           aPort,
                                                             GpSocketIPv        aIPv,
                                                             milliseconds_t     aConnectTimeout);
    void                        ConnectTcpAndWait           (const GpSocketAddr&    aServerAddr,
                                                             milliseconds_t         aConnectTimeout);

protected:
    virtual void                OnStart                     (void) override;
    virtual void                OnStop                      (StopExceptionsT& aStopExceptionsOut) noexcept override;
    virtual void                OnStopException             (const GpException& aException) noexcept override = 0;

    virtual void                OnReadyToRead               (GpSocket& aSocket) override = 0;
    virtual void                OnReadyToWrite              (GpSocket& aSocket) override = 0;
    virtual void                OnClosed                    (GpSocket& aSocket) override = 0;
    virtual void                OnError                     (GpSocket& aSocket) override = 0;
    virtual void                OnConnected                 (GpSocketTCP& aSocket) = 0;
    virtual void                ProcessOtherMessages        (GpAny& aMessage) override;
    virtual GpSocket::SP        FindSocket                  (GpSocketId aSocketId) override final;

    inline const GpSocketTCP&   SocketTCP                   (void) const;
    inline GpSocketTCP&         SocketTCP                   (void);

    void                        CheckBeforeNewTcpConnection (const GpSocketAddr& aServerAddr) REQUIRES(iSocketSpinLock);
    void                        _CloseConnection            (void) REQUIRES(iSocketSpinLock);

private:
    const GpSocketFlags         iSocketFlags;
    const GpIOEventPollerIdx    iIOEventPollerIdx;

    mutable GpSpinLock          iSocketSpinLock;
    ConnectionStateTE           iConnectionState                GUARDED_BY(iSocketSpinLock) = ConnectionStateT::NOT_CONNECTED;
    GpSocketAddr                iConnectedToAddr                GUARDED_BY(iSocketSpinLock);
    GpSocketTCP::SP             iSocketTCP                      GUARDED_BY(iSocketSpinLock);
    GpTaskId                    iIOEventPollerSubscribeTaskId   GUARDED_BY(iSocketSpinLock) = {};
};

const GpSocketTCP&  GpTcpClientTask::SocketTCP (void) const
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};
    return iSocketTCP.V();
}

GpSocketTCP&    GpTcpClientTask::SocketTCP (void)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};
    return iSocketTCP.V();
}

}// namespace GPlatform

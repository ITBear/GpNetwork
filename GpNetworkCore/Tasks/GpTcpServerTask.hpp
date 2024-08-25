#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSocketsTask.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpTcpServerTask: public GpSocketsTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTcpServerTask)
    CLASS_DD(GpTcpServerTask)

protected:
                                GpTcpServerTask         (GpSocketTCP::SP    aSocketTCP,
                                                         GpIOEventPollerIdx aIOEventPollerIdx);
                                GpTcpServerTask         (GpSocketTCP::SP    aSocketTCP,
                                                         GpIOEventPollerIdx aIOEventPollerIdx,
                                                         std::string        aTaskName);

public:
    virtual                     ~GpTcpServerTask        (void) noexcept override;

protected:
    virtual void                OnStart                 (void) override;
    virtual void                OnStop                  (StopExceptionsT& aStopExceptionsOut) noexcept override;

    virtual void                OnReadyToRead           (GpSocket& aSocket) override = 0;
    virtual void                OnReadyToWrite          (GpSocket& aSocket) override = 0;
    virtual void                OnClosed                (GpSocket& aSocket) override = 0;
    virtual void                OnError                 (GpSocket& aSocket) override = 0;
    virtual void                ProcessOtherMessages    (GpAny& aMessage) override;
    virtual GpSocket::SP        FindSocket              (GpSocketId aSocketId) override final;

    inline const GpSocketTCP&   SocketTCP               (void) const;
    inline GpSocketTCP&         SocketTCP               (void);
    void                        _CloseConnection        (void) REQUIRES(iSocketSpinLock);

private:
    const GpSocketFlags         iSocketFlags;
    const GpIOEventPollerIdx    iIOEventPollerIdx;

    mutable GpSpinLock          iSocketSpinLock;
    GpSocketTCP::SP             iSocketTCP                      GUARDED_BY(iSocketSpinLock);
    GpTaskId                    iIOEventPollerSubscribeTaskId   GUARDED_BY(iSocketSpinLock) = {};
};

const GpSocketTCP&  GpTcpServerTask::SocketTCP (void) const
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};
    return iSocketTCP.V();
}

GpSocketTCP&    GpTcpServerTask::SocketTCP (void)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSocketSpinLock};
    return iSocketTCP.V();
}

}// namespace GPlatform

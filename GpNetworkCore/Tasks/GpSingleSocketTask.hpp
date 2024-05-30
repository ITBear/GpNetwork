#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSocketsTask.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketFactory.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSingleSocketTask: public GpSocketsTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSingleSocketTask)
    CLASS_DD(GpSingleSocketTask)

protected:
                                GpSingleSocketTask      (GpSocketFactory::SP    aSocketFactory,
                                                         GpIOEventPollerIdx     aIOEventPollerIdx) noexcept;
                                GpSingleSocketTask      (GpSocketFactory::SP    aSocketFactory,
                                                         GpIOEventPollerIdx     aIOEventPollerIdx,
                                                         std::string            aTaskName) noexcept;
                                GpSingleSocketTask      (GpSocket::SP       aSocket,
                                                         GpIOEventPollerIdx aIOEventPollerIdx) noexcept;
                                GpSingleSocketTask      (GpSocket::SP       aSocket,
                                                         GpIOEventPollerIdx aIOEventPollerIdx,
                                                         std::string        aTaskName) noexcept;

public:
    virtual                     ~GpSingleSocketTask     (void) noexcept override;

protected:
    virtual void                OnStart                 (void) override final;
    virtual GpException::C::Opt OnStop                  (void) noexcept override final;

    virtual void                OnReadyToRead           (GpSocket& aSocket) override = 0;
    virtual void                OnReadyToWrite          (GpSocket& aSocket) override = 0;
    virtual void                OnClosed                (GpSocket& aSocket) override = 0;
    virtual void                OnError                 (GpSocket& aSocket) override = 0;
    virtual void                ProcessOtherMessages    (GpAny& aMessage) override = 0;

    virtual GpSocket::SP        FindSocket              (const GpSocketId aSocketId) override final;

    const GpSocket&             Socket                  (void) const {return iSocket.V();}
    GpSocket&                   Socket                  (void) {return iSocket.V();}
    GpSocket::SP                SocketSP                (void) {return iSocket;}
    const GpIOEventPollerIdx    IOEventPollerIdx        (void) const noexcept {return iIOEventPollerIdx;}

private:
    GpSocketFactory::SP         iSocketFactory;
    GpSocket::SP                iSocket;
    const GpIOEventPollerIdx    iIOEventPollerIdx;
};

}// namespace GPlatform

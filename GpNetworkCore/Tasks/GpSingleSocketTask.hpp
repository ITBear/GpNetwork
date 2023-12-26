#pragma once

#include "GpSocketsTask.hpp"
#include "../Sockets/GpSocketFactory.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpSingleSocketTask: public GpSocketsTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSingleSocketTask)
    CLASS_DD(GpSingleSocketTask)

protected:
    inline                              GpSingleSocketTask  (GpSocketFactory::SP    aSocketFactory) noexcept;
    inline                              GpSingleSocketTask  (GpSocketFactory::SP    aSocketFactory,
                                                             std::u8string          aName) noexcept;
    inline                              GpSingleSocketTask  (GpSocket::SP   aSocket) noexcept;
    inline                              GpSingleSocketTask  (GpSocket::SP   aSocket,
                                                             std::u8string  aName) noexcept;

public:
    virtual                             ~GpSingleSocketTask (void) noexcept override;

protected:
    virtual void                        OnStart             (void) override final;
    virtual std::optional<GpException>  OnStop              (void) noexcept override final;

    virtual void                        OnReadyToRead       (GpSocket& aSocket) override = 0;
    virtual void                        OnReadyToWrite      (GpSocket& aSocket) override = 0;
    virtual void                        OnClosed            (GpSocket& aSocket) override = 0;
    virtual void                        OnError             (GpSocket& aSocket) override = 0;

    virtual GpSocket::SP                FindSocket          (const GpIOObjectId aSocketId) override final;

    GpSocket&                           Socket              (void) {return iSocket.V();}

private:
    GpSocketFactory::SP                 iSocketFactory;
    GpSocket::SP                        iSocket;
};

GpSingleSocketTask::GpSingleSocketTask (GpSocketFactory::SP aSocketFactory) noexcept:
iSocketFactory(std::move(aSocketFactory))
{
}

GpSingleSocketTask::GpSingleSocketTask
(
    GpSocketFactory::SP aSocketFactory,
    std::u8string       aName
) noexcept:
GpSocketsTask(std::move(aName)),
iSocketFactory(std::move(aSocketFactory))
{
}

GpSingleSocketTask::GpSingleSocketTask
(
    GpSocket::SP    aSocket,
    std::u8string   aName
) noexcept:
GpSocketsTask(std::move(aName)),
iSocket(std::move(aSocket))
{
}

GpSingleSocketTask::GpSingleSocketTask (GpSocket::SP aSocket) noexcept:
iSocket(std::move(aSocket))
{
}

}// namespace GPlatform

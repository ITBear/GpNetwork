#pragma once

#include "../Sockets/GpSocket.hpp"
#include "../Sockets/GpSocketFactory.hpp"
#include "../../../GpCore2/GpTasks/Fibers/GpTaskFiber.hpp"
#include "../Pollers/GpIOEventType.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketTask: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSocketTask)
    CLASS_DD(GpSocketTask)

public:
    inline                              GpSocketTask    (GpSocketFactory::SP    aSocketFactory) noexcept;
    inline                              GpSocketTask    (GpSocketFactory::SP    aSocketFactory,
                                                         std::u8string          aName) noexcept;
    inline                              GpSocketTask    (GpSocket::SP   aSocket) noexcept;
    inline                              GpSocketTask    (GpSocket::SP   aSocket,
                                                         std::u8string  aName) noexcept;
    virtual                             ~GpSocketTask   (void) noexcept override;

protected:
    virtual void                        OnStart         (void) override final;
    virtual GpTaskRunRes::EnumT         OnStep          (void) override final;
    virtual std::optional<GpException>  OnStop          (void) noexcept override final;

    virtual GpTaskRunRes::EnumT         OnReadyToRead   (GpSocket& aSocket) = 0;
    virtual GpTaskRunRes::EnumT         OnReadyToWrite  (GpSocket& aSocket) = 0;
    virtual void                        OnClosed        (GpSocket& aSocket) = 0;
    virtual void                        OnError         (GpSocket& aSocket) = 0;

private:
    GpTaskRunRes::EnumT                 ProcessEvents   (const GpIOEventsTypes aIoEvents);

private:
    GpSocketFactory::SP                 iSocketFactory;
    GpSocket::SP                        iSocket;
};

GpSocketTask::GpSocketTask (GpSocketFactory::SP aSocketFactory) noexcept:
iSocketFactory(std::move(aSocketFactory))
{
}

GpSocketTask::GpSocketTask
(
    GpSocketFactory::SP aSocketFactory,
    std::u8string       aName
) noexcept:
GpTaskFiber(std::move(aName)),
iSocketFactory(std::move(aSocketFactory))
{
}

GpSocketTask::GpSocketTask
(
    GpSocket::SP    aSocket,
    std::u8string   aName
) noexcept:
GpTaskFiber(std::move(aName)),
iSocket(std::move(aSocket))
{
}

GpSocketTask::GpSocketTask (GpSocket::SP aSocket) noexcept:
iSocket(std::move(aSocket))
{
}

}//namespace GPlatform

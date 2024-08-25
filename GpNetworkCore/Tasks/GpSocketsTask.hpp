#pragma once

#include <GpNetwork/GpNetworkCore/Sockets/GpSocket.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventType.hpp>

#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <boost/container/flat_map.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketsTask: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSocketsTask)
    CLASS_DD(GpSocketsTask)
    TAG_SET(THREAD_SAFE)

protected:
                                GpSocketsTask           (void) noexcept;
                                GpSocketsTask           (std::string aTaskName) noexcept;

public:
    virtual                     ~GpSocketsTask          (void) noexcept override;

protected:
    virtual void                OnStart                 (void) override;
    virtual GpTaskRunRes::EnumT OnStep                  (void) override final;
    virtual void                OnStop                  (StopExceptionsT& aStopExceptionsOut) noexcept override;
    virtual void                OnStopException         (const GpException& aException) noexcept override = 0;

    virtual void                OnReadyToRead           (GpSocket& aSocket) = 0;
    virtual void                OnReadyToWrite          (GpSocket& aSocket) = 0;
    virtual void                OnClosed                (GpSocket& aSocket) = 0;
    virtual void                OnError                 (GpSocket& aSocket) = 0;
    virtual void                ProcessOtherMessages    (GpAny& aMessage) = 0;
    virtual GpSocket::SP        FindSocket              (GpSocketId aSocketId) = 0;

private:
    void                        ProcessSocketEvents     (GpSocketId         aSocketId,
                                                         GpIOEventsTypes    aIoEvents);
};

}// namespace GPlatform

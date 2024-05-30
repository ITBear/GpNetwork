#pragma once

#include "../Sockets/GpSocket.hpp"

#include "../Pollers/GpIOEventType.hpp"

#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <boost/container/flat_map.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketsTask: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSocketsTask)
    CLASS_DD(GpSocketsTask)

protected:
                                GpSocketsTask           (void) noexcept = default;
    inline                      GpSocketsTask           (std::string aTaskName) noexcept;

public:
    virtual                     ~GpSocketsTask          (void) noexcept override;

protected:
    virtual void                OnStart                 (void) override;
    virtual GpTaskRunRes::EnumT OnStep                  (void) override final;
    virtual GpException::C::Opt OnStop                  (void) noexcept override;

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

GpSocketsTask::GpSocketsTask (std::string aTaskName) noexcept:
GpTaskFiber{std::move(aTaskName)}
{
}

}// namespace GPlatform

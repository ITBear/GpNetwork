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
    inline                              GpSocketsTask       (void) noexcept = default;
    inline                              GpSocketsTask       (std::u8string  aName) noexcept;

public:
    virtual                             ~GpSocketsTask      (void) noexcept override;

protected:
    virtual void                        OnStart             (void) override;
    virtual GpTaskRunRes::EnumT         OnStep              (void) override final;
    virtual std::optional<GpException>  OnStop              (void) noexcept override;

    virtual void                        OnReadyToRead       (GpSocket& aSocket) = 0;
    virtual void                        OnReadyToWrite      (GpSocket& aSocket) = 0;
    virtual void                        OnClosed            (GpSocket& aSocket) = 0;
    virtual void                        OnError             (GpSocket& aSocket) = 0;
    virtual GpSocket::SP                FindSocket          (const GpIOObjectId aSocketId) = 0;

protected:
    virtual void                        ProcessOtherEvents  (GpAny& aEvent);

private:
    void                                ProcessSocketEvents (const GpIOObjectId     aSocketId,
                                                             const GpIOEventsTypes  aIoEvents);
};

GpSocketsTask::GpSocketsTask (std::u8string aName) noexcept:
GpTaskFiber(std::move(aName))
{
}

}// namespace GPlatform

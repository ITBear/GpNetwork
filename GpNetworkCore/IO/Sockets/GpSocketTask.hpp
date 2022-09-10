#pragma once

#include "GpSocket.hpp"
#include "../Events/GpIOEventPoller.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketTask: public GpLogTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSocketTask)
    CLASS_DECLARE_DEFAULTS(GpSocketTask)

public:
    inline                  GpSocketTask        (std::string        aName,
                                                 GpIOEventPoller&   aIOPoller,
                                                 GpSocket::SP       aSocket) noexcept;
    virtual                 ~GpSocketTask       (void) noexcept override;

    const GpSocket&         Socket              (void) const noexcept {return iSocket.Vn();}
    GpSocket&               Socket              (void) noexcept {return iSocket.Vn();}

protected:
    GpIOEventPoller&        IOPoller            (void) {return iIOPoller;}

    virtual void            OnStart             (void) override;
    virtual GpTaskDoRes     OnStep              (EventOptRefT aEvent) override;
    virtual void            OnStop              (void) noexcept override;

    virtual GpTaskDoRes     OnSockReadyToRead   (GpSocket& aSocket) = 0;
    virtual GpTaskDoRes     OnSockReadyToWrite  (GpSocket& aSocket) = 0;
    virtual void            OnSockClosed        (GpSocket& aSocket) = 0;
    virtual void            OnSockError         (GpSocket& aSocket) = 0;

private:
    GpTaskDoRes             ProcessIOEvent      (const GpIOEvent& aIOEvent);

private:
    GpIOEventPoller&        iIOPoller;
    GpSocket::SP            iSocket;
};

GpSocketTask::GpSocketTask
(
    std::string         aName,
    GpIOEventPoller&    aIOPoller,
    GpSocket::SP        aSocket
) noexcept:
GpLogTaskFiberBase(std::move(aName)),
iIOPoller(aIOPoller),
iSocket  (std::move(aSocket))
{
}

}//namespace GPlatform

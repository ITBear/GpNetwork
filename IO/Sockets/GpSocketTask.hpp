#pragma once

#include "GpSocket.hpp"
#include "../Events/GpIOEventPoller.hpp"

namespace GPlatform {

class GPNETWORK_API GpSocketTask: public GpTaskBase
{
public:
    CLASS_REMOVE_CTRS(GpSocketTask)
    CLASS_DECLARE_DEFAULTS(GpSocketTask)

public:
                        GpSocketTask        (GpIOEventPoller::WP    aIOPooler,
                                             GpSocket::SP           aSocket) noexcept;
    virtual             ~GpSocketTask       (void) noexcept override;

    const GpSocket&     Socket              (void) const noexcept {return iSocket.VCn();}
    GpSocket&           Socket              (void) noexcept {return iSocket.Vn();}

protected:
    GpIOEventPoller::WP IOPooler            (void) {return iIOPooler;}

    virtual void        OnStart             (void) override;
    virtual ResT        OnStep              (EventOptRefT aEvent) override;
    virtual void        OnStop              (void) noexcept override;

    virtual ResT        OnSockReadyToRead   (GpSocket& aSocket) = 0;
    virtual ResT        OnSockReadyToWrite  (GpSocket& aSocket) = 0;
    virtual void        OnSockClosed        (GpSocket& aSocket) = 0;
    virtual void        OnSockError         (GpSocket& aSocket) = 0;

private:
    GpIOEventPoller::WP iIOPooler;
    GpSocket::SP        iSocket;
};

}//namespace GPlatform

#pragma once

#include "GpSocket.hpp"
#include "../Events/GpIOEventPoller.hpp"

namespace GPlatform {

class GPNETWORK_API GpSocketTask: public GpTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSocketTask)
    CLASS_DECLARE_DEFAULTS(GpSocketTask)

public:
                            GpSocketTask        (std::string_view       aName,
                                                 GpIOEventPoller::WP    aIOPoller,
                                                 GpSocket::SP           aSocket);
    virtual                 ~GpSocketTask       (void) noexcept override;

    const GpSocket&         Socket              (void) const noexcept {return iSocket.VCn();}
    GpSocket&               Socket              (void) noexcept {return iSocket.Vn();}

protected:
    GpIOEventPoller::WP     IOPoller            (void) {return iIOPoller;}

    virtual void            OnStart             (void) override;
    virtual GpTask::ResT    OnStep              (EventOptRefT aEvent) override;
    virtual void            OnStop              (void) noexcept override;

    virtual GpTask::ResT    OnSockReadyToRead   (GpSocket& aSocket) = 0;
    virtual GpTask::ResT    OnSockReadyToWrite  (GpSocket& aSocket) = 0;
    virtual void            OnSockClosed        (GpSocket& aSocket) = 0;
    virtual void            OnSockError         (GpSocket& aSocket) = 0;

private:
    GpTask::ResT            ProcessIOEvent      (const GpIOEvent& aIOEvent);

private:
    GpIOEventPoller::WP     iIOPoller;
    GpSocket::SP            iSocket;
};

}//namespace GPlatform

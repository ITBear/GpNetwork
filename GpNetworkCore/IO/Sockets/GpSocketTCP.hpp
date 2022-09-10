#pragma once

#include "GpSocket.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketTCP final: public GpSocket
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSocketTCP)
    CLASS_DECLARE_DEFAULTS(GpSocketTCP)

    enum class StateT
    {
        NOT_CONNECTED,
        LISTEN,
        CONNECTION_IN_PROGRESS,
        OUTGOING,
        INCOMING
    };

public:
    inline                  GpSocketTCP             (const GpSocketFlags&   aFlags,
                                                     const CloseModeT       aCloseMode) noexcept;
    virtual                 ~GpSocketTCP            (void) noexcept override final;

    static GpSocketTCP::SP  SFromID                 (GpSocketAddr::SocketIdT    aId,
                                                     const CloseModeT           aCloseMode,
                                                     const StateT               aState);

    StateT                  State                   (void) const noexcept {return iState;}

    void                    Listen                  (const GpSocketAddr&    aAddr,
                                                     const size_t           aMaxQueueSize);
    void                    Connect                 (const GpSocketAddr&    aAddr,
                                                     const milliseconds_t   aTimeout);
    GpSocketTCP::SP         Accept                  (const GpSocketFlags& aFlags);

    virtual size_t          Read                    (GpByteWriter& aWriter) override final;
    virtual size_t          Write                   (GpByteReader& aReader) override final;

private:
    void                    SetFromRawTCP           (const GpSocketAddr::SocketIdT  aId,
                                                     const StateT                   aState);

    void                    ConnectSync             (const GpSocketAddr&    aAddr);
    void                    ConnectAsync            (const GpSocketAddr&    aAddr);

    void                    SetUserTimeout          (const milliseconds_t aTimeout);

private:
    StateT                  iState  = StateT::NOT_CONNECTED;
};

GpSocketTCP::GpSocketTCP
(
    const GpSocketFlags&    aFlags,
    const CloseModeT        aCloseMode
) noexcept:
GpSocket(ProtocolT::TCP, aFlags, aCloseMode)
{
}

}//namespace GPlatform

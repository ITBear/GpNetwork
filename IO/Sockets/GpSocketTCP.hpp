#pragma once

#include "GpSocket.hpp"

namespace GPlatform {

class GPNETWORK_API GpSocketTCP final: public GpSocket
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
                            GpSocketTCP             (const GpSocketFlags&   aFlags,
                                                     const CloseModeT       aCloseMode) noexcept;
    virtual                 ~GpSocketTCP            (void) noexcept override final;

    static GpSocketTCP::SP  SFromID                 (GpSocketAddr::SocketIdT    aId,
                                                     const CloseModeT           aCloseMode,
                                                     const StateT               aState);

    StateT                  State                   (void) const noexcept {return iState;}

    void                    Listen                  (const GpSocketAddr&    aAddr,
                                                     const count_t          aMaxQueueSize);
    void                    Connect                 (const GpSocketAddr&    aAddr,
                                                     const milliseconds_t   aTimeout);
    GpSocketTCP::SP         Accept                  (const GpSocketFlags& aFlags);

    virtual size_byte_t     Read                    (GpByteWriter& aWriter) override final;
    virtual size_byte_t     Write                   (GpByteReader& aReader) override final;

private:
    void                    SetFromRawTCP           (const GpSocketAddr::SocketIdT  aId,
                                                     const StateT                   aState);

    void                    ConnectSync             (const GpSocketAddr&    aAddr);
    void                    ConnectAsync            (const GpSocketAddr&    aAddr);

    void                    SetUserTimeout          (const milliseconds_t aTimeout);

private:
    StateT                  iState  = StateT::NOT_CONNECTED;
};

}//namespace GPlatform

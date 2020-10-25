#pragma once

#include "GpSocket.hpp"

namespace GPlatform {

class GPNETWORK_API GpSocketTCP final: public GpSocket
{
public:
    CLASS_REMOVE_CTRS(GpSocketTCP)
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
                            GpSocketTCP             (const GpSocketFlags& aFlags) noexcept;
    virtual                 ~GpSocketTCP            (void) noexcept override final;

    StateT                  State                   (void) const noexcept {return iState;}

    void                    Listen                  (const GpSocketAddr&    aAddr,
                                                     const count_t          aMaxQueueSize);
    void                    Connect                 (const GpSocketAddr&    aAddr,
                                                     const milliseconds_t   aTimeout);
    GpSocketTCP::SP         Accept                  (void);

    virtual size_byte_t     Read                    (GpByteWriter& aWriter) override final;
    virtual size_byte_t     Write                   (GpByteReader& aReader) override final;

private:
    void                    SetFromIncomingRawId    (const GpSocketAddr::SocketIdT aId);

    void                    ConnectSync             (const GpSocketAddr&    aAddr);
    void                    ConnectAsync            (const GpSocketAddr&    aAddr);

    void                    SetUserTimeout          (const milliseconds_t aTimeout);

private:
    StateT                  iState  = StateT::NOT_CONNECTED;
};

}//namespace GPlatform

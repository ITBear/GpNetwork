#pragma once

#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReaderStorage.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocket.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_CORE_API, GpSocketStateTCP,
    NOT_CONNECTED,
    LISTEN,
    CONNECTION_IN_PROGRESS,
    OUTGOING,
    INCOMING
);

class GP_NETWORK_CORE_API GpSocketTCP final: public GpSocket
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_COPY(GpSocketTCP)
    CLASS_DD(GpSocketTCP)

    using StateE = GpSocketStateTCP::EnumT;

public:
                                GpSocketTCP     (GpSocketTCP&& aSocket) noexcept;
                                GpSocketTCP     (GpSocketFlags  aFlags,
                                                 CloseModeT     aCloseMode) noexcept;
    virtual                     ~GpSocketTCP    (void) noexcept override final;

    GpSocketTCP&                operator=       (GpSocketTCP&& aSocket);
    void                        Set             (GpSocketTCP&& aSocket);

    static GpSocketTCP::SP      SFromID         (GpSocketId aId,
                                                 CloseModeT aCloseMode,
                                                 StateE     aState);

    StateE                      State           (void) const noexcept {return iState;}

    void                        Listen          (const GpSocketAddr&    aAddr,
                                                 size_t                 aMaxQueueSize);
    [[nodiscard]] StateE        Connect         (const GpSocketAddr&    aAddr);
    void                        OnConnected     (void);
    void                        OnConnectionFailed(void);
    GpSocketTCP::C::Opts::Val   Accept          (const GpSocketFlags& aFlags);

    [[nodiscard]] size_t        Read            (GpByteWriter& aWriter);
    GpByteArray                 Read            (void);
    [[nodiscard]] size_t        Write           (GpByteReader& aReader);
    [[nodiscard]] size_t        Write           (GpSpanByteR aData);

    bool                        IsConnected     (void) const noexcept;
    static bool                 SIsConnected    (GpSocketId aId) noexcept;

private:
    void                        SetFromRawTCP   (GpSocketId aId,
                                                 StateE     aState);

    void                        ConnectSync     (const GpSocketAddr&    aAddr);
    StateE                      ConnectAsync    (const GpSocketAddr&    aAddr);

    void                        SetUserTimeout  (milliseconds_t aTimeout);
    void                        SetFlag_NoDelay (bool aValue);

private:
    StateE iState = StateE::NOT_CONNECTED;
};

}// namespace GPlatform

#pragma once

#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReaderStorage.hpp>

#include <GpNetwork/GpNetworkCore/Sockets/GpSocket.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketTCP final: public GpSocket
{
public:
    CLASS_REMOVE_CTRS_COPY(GpSocketTCP)
    CLASS_DD(GpSocketTCP)

    enum class StateT
    {
        NOT_CONNECTED,
        LISTEN,
        CONNECTION_IN_PROGRESS,
        OUTGOING,
        INCOMING
    };

public:
    inline                      GpSocketTCP         (void) noexcept;
    inline                      GpSocketTCP         (GpSocketTCP&& aSocket) noexcept;
    inline                      GpSocketTCP         (GpSocketFlags  aFlags,
                                                     CloseModeT     aCloseMode) noexcept;
    virtual                     ~GpSocketTCP        (void) noexcept override final;

    inline GpSocketTCP&         operator=           (GpSocketTCP&& aSocket);
    inline void                 Set                 (GpSocketTCP&& aSocket);

    static GpSocketTCP::SP      SFromID             (GpSocketId aId,
                                                     CloseModeT aCloseMode,
                                                     StateT     aState);

    StateT                      State               (void) const noexcept {return iState;}

    void                        Listen              (const GpSocketAddr&    aAddr,
                                                     size_t                 aMaxQueueSize);
    void                        ConnectAndWait      (const GpSocketAddr&    aAddr,
                                                     milliseconds_t         aTimeout,
                                                     GpIOEventPollerIdx     aIOEventPollerIdx,
                                                     GpTaskId               aIOEventPollerSubscribeTaskId);
    GpSocketTCP::C::Opt::Val    Accept              (const GpSocketFlags& aFlags);

    [[nodiscard]] size_t        Read                (GpByteWriter& aWriter);
    [[nodiscard]] size_t        Write               (GpByteReader& aReader);

    inline GpBytesArray         Read                (void);
    [[nodiscard]] inline size_t Write               (GpSpanByteR    aData);

    bool                        IsConnected         (void) const noexcept;
    static bool                 SIsConnected        (GpSocketId aId) noexcept;

private:
    void                        SetFromRawTCP       (GpSocketId aId,
                                                     StateT     aState);

    void                        ConnectSync         (const GpSocketAddr&    aAddr);
    void                        ConnectAsyncAndWait (const GpSocketAddr&        aAddr,
                                                     const GpIOEventPollerIdx   aIOEventPollerIdx,
                                                     const GpTaskId             aIOEventPollerSubscribeTaskId);

    void                        SetUserTimeout      (milliseconds_t aTimeout);
    inline void                 SetFlag_NoDelay     (bool aValue);

private:
    StateT                      iState  = StateT::NOT_CONNECTED;
};

GpSocketTCP::GpSocketTCP (void) noexcept
{
}

GpSocketTCP::GpSocketTCP (GpSocketTCP&& aSocket) noexcept:
GpSocket{std::move(aSocket)},
iState{aSocket.iState}
{
}

GpSocketTCP::GpSocketTCP
(
    const GpSocketFlags aFlags,
    const CloseModeT    aCloseMode
) noexcept:
GpSocket
{
    ProtocolT::TCP,
    aFlags,
    aCloseMode
}
{
}

GpSocketTCP&    GpSocketTCP::operator= (GpSocketTCP&& aSocket)
{
    Set(std::move(aSocket));

    return *this;
}

void    GpSocketTCP::Set (GpSocketTCP&& aSocket)
{
    GpSocket::Set(std::move(aSocket));
    iState = aSocket.State();
    aSocket.iState = StateT::NOT_CONNECTED;
}

GpBytesArray    GpSocketTCP::Read (void)
{
    GpBytesArray dataBuffer;

    GpByteWriterStorageByteArray    dataWriterStorage(dataBuffer);
    GpByteWriter                    dataWriter(dataWriterStorage);

    std::ignore = Read(dataWriter);

    dataWriter.OnEnd();

    return dataBuffer;
}

size_t  GpSocketTCP::Write (GpSpanByteR aData)
{
    GpByteReaderStorage dataReaderStorage(aData);
    GpByteReader        dataReader(dataReaderStorage);

    return Write(dataReader);
}

void    GpSocketTCP::SetFlag_NoDelay (bool aValue)
{
    int option = aValue ? 1 : 0;
    //const int res = setsockopt(Id(), SOL_TCP, TCP_NODELAY, &option, sizeof(option));
    const int res = setsockopt(Id(), IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&option), sizeof(option));

    SCheckResOrThrow(res, {});
}

}// namespace GPlatform

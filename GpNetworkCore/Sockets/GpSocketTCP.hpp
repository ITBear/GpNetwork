#pragma once

#include "GpSocket.hpp"
#include "../../../GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp"

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
    inline                          GpSocketTCP     (void) noexcept;
    inline                          GpSocketTCP     (GpSocketTCP&& aSocket) noexcept;
    inline                          GpSocketTCP     (const GpSocketFlags&   aFlags,
                                                     const CloseModeT       aCloseMode) noexcept;
    virtual                         ~GpSocketTCP    (void) noexcept override final;

    inline GpSocketTCP&             operator=       (GpSocketTCP&& aSocket);
    inline void                     Set             (GpSocketTCP&& aSocket);

    static GpSocketTCP::SP          SFromID         (GpIOObjectId       aId,
                                                     const CloseModeT   aCloseMode,
                                                     const StateT       aState);

    StateT                          State           (void) const noexcept {return iState;}

    void                            Listen          (const GpSocketAddr&    aAddr,
                                                     const size_t           aMaxQueueSize);
    void                            Connect         (const GpSocketAddr&    aAddr,
                                                     const milliseconds_t   aTimeout);
    GpSocketTCP::C::Opt::Val        Accept          (const GpSocketFlags& aFlags);

    [[nodiscard]] size_t            Read            (GpByteWriter& aWriter);
    [[nodiscard]] size_t            Write           (GpByteReader& aReader);

    inline GpBytesArray             Read            (void);
    [[nodiscard]] inline size_t     Write           (GpSpanPtrByteR aData);

private:
    void                            SetFromRawTCP   (const GpIOObjectId aId,
                                                     const StateT       aState);

    void                            ConnectSync     (const GpSocketAddr&    aAddr);
    void                            ConnectAsync    (const GpSocketAddr&    aAddr);

    void                            SetUserTimeout  (const milliseconds_t aTimeout);

private:
    StateT                          iState  = StateT::NOT_CONNECTED;
};

GpSocketTCP::GpSocketTCP (void) noexcept:
GpSocket()
{
}

GpSocketTCP::GpSocketTCP (GpSocketTCP&& aSocket) noexcept:
GpSocket(std::move(aSocket)),
iState(aSocket.iState)
{
}

GpSocketTCP::GpSocketTCP
(
    const GpSocketFlags&    aFlags,
    const CloseModeT        aCloseMode
) noexcept:
GpSocket(ProtocolT::TCP, aFlags, aCloseMode)
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
    dataBuffer.resize(dataWriter.TotalWrite());

    return dataBuffer;
}

size_t  GpSocketTCP::Write (GpSpanPtrByteR aData)
{
    GpByteReaderStorage dataReaderStorage(aData);
    GpByteReader        dataReader(dataReaderStorage);

    return Write(dataReader);
}

}//namespace GPlatform

#pragma once

#include <GpCore2/Config/IncludeExt/fmt.hpp>

#include "GpSocketAddr.hpp"

namespace GPlatform {

class GpSocketMessageUDP
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSocketMessageUDP)
    CLASS_DD(GpSocketMessageUDP)

public:
                                GpSocketMessageUDP          (void) = default;

    inline const GpSocketAddr&  Addr                        (void) const noexcept;
    inline GpSocketAddr&        Addr                        (void) noexcept;
    inline GpSocketMessageUDP&  SetAddr                     (const GpSocketAddr& aAddr) noexcept;

    inline GpSpanByteR          Data_                       (void) const noexcept;
    inline GpSpanByteRW         Data_                       (void) noexcept;
    inline GpSpanByteR          DataUseOnly                 (void) const noexcept;
    inline GpSpanByteRW         DataUseOnly                 (void) noexcept;
    inline size_t               DataUseSize                 (void) const;
    inline GpSocketMessageUDP&  SetDataUseSize              (size_t aSize);

    inline GpSpanByteR          ControlBuffer               (void) const noexcept;
    inline GpSpanByteRW         ControlBuffer               (void) noexcept;
    inline GpSpanByteR          ControlBufferUseOnly        (void) const noexcept;
    inline GpSpanByteRW         ControlBufferUseOnly        (void) noexcept;
    inline size_t               ControlBufferUseSize        (void) const noexcept;
    inline GpSocketMessageUDP&  SetControlBufferUseSize     (size_t aSize);


    inline int                  Flags                       (void) const noexcept;
    inline GpSocketMessageUDP&  SetFlags                    (int aFalgs) noexcept;

private:
    GpSocketAddr                                            iAddr;
    std::array<std::byte, GP_NETWORK_UDP_PACKET_MAX_SIZE>   iData;
    size_t                                                  iDataUseSize            = 0;
    std::array<std::byte, GP_NETWORK_CONTROL_INFO_MAX_SIZE> iControlBuffer;
    size_t                                                  iControlBufferUseSize   = 0;
    int                                                     iFlags                  = 0;
};

const GpSocketAddr& GpSocketMessageUDP::Addr (void) const noexcept
{
    return iAddr;
}

GpSocketAddr&   GpSocketMessageUDP::Addr (void) noexcept
{
    return iAddr;
}

GpSocketMessageUDP& GpSocketMessageUDP::SetAddr (const GpSocketAddr& aAddr) noexcept
{
    iAddr = aAddr;

    return *this;
}

GpSpanByteR GpSocketMessageUDP::Data_ (void) const noexcept
{
    return iData;
}

GpSpanByteRW    GpSocketMessageUDP::Data_ (void) noexcept
{
    return iData;
}

GpSpanByteR     GpSocketMessageUDP::DataUseOnly (void) const noexcept
{
    return GpSpanByteR(std::data(iData), iDataUseSize);
}

GpSpanByteRW    GpSocketMessageUDP::DataUseOnly (void) noexcept
{
    return GpSpanByteRW(std::data(iData), iDataUseSize);
}

size_t  GpSocketMessageUDP::DataUseSize (void) const
{
    return iDataUseSize;
}

GpSocketMessageUDP& GpSocketMessageUDP::SetDataUseSize (const size_t aSize)
{
    THROW_COND_GP
    (
        aSize <= std::size(iData),
        [&]()
        {
            return fmt::format
            (
                "Data payload size is out of range. Max size {}, required size {}",
                std::size(iData),
                aSize
            );
        }
    );

    iDataUseSize = aSize;

    return *this;
}

GpSpanByteR GpSocketMessageUDP::ControlBuffer (void) const noexcept
{
    return iControlBuffer;
}

GpSpanByteRW    GpSocketMessageUDP::ControlBuffer (void) noexcept
{
    return iControlBuffer;
}

GpSpanByteR GpSocketMessageUDP::ControlBufferUseOnly (void) const noexcept
{
    return GpSpanByteR(std::data(iControlBuffer), iControlBufferUseSize);
}

GpSpanByteRW    GpSocketMessageUDP::ControlBufferUseOnly (void) noexcept
{
    return GpSpanByteRW(std::data(iControlBuffer), iControlBufferUseSize);
}

size_t  GpSocketMessageUDP::ControlBufferUseSize (void) const noexcept
{
    return iControlBufferUseSize;
}

GpSocketMessageUDP& GpSocketMessageUDP::SetControlBufferUseSize (const size_t aSize)
{
    THROW_COND_GP
    (
        aSize <= std::size(iControlBuffer),
        [&]()
        {
            return fmt::format
            (
                "Control buffer size is out of range. Max size {}, required size {}",
                std::size(iControlBuffer),
                aSize
            );
        }
    );

    iControlBufferUseSize = aSize;

    return *this;
}

int GpSocketMessageUDP::Flags (void) const noexcept
{
    return iFlags;
}

GpSocketMessageUDP& GpSocketMessageUDP::SetFlags (const int aFalgs) noexcept
{
    iFlags = aFalgs;

    return *this;
}

}// namespace GPlatform

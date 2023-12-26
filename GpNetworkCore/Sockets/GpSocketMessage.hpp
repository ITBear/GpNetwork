#pragma once

#include "GpSocketAddr.hpp"

#include <fmt/core.h>

namespace GPlatform {

class GpSocketMessage
{
public:
    inline const GpSocketAddr&  Addr                (void) const noexcept;
    inline GpSocketAddr&        Addr                (void) noexcept;
    inline GpSocketMessage&     SetAddr             (const GpSocketAddr& aAddr) noexcept;

    inline GpSpanPtrByteR       DataPayload         (void) const noexcept;
    inline GpSpanPtrByteRW      DataPayload         (void) noexcept;
    inline GpSpanPtrByteRW      DataStorage         (void) noexcept;
    inline GpSocketMessage&     SetDataPayloadSize  (const size_t aSize);

    inline GpSpanPtrByteR       ControlPayload      (void) const noexcept;
    inline GpSpanPtrByteRW      ControlPayload      (void) noexcept;
    inline GpSpanPtrByteRW      ControlStorage      (void) noexcept;

    inline int                  Flags               (void) const noexcept;
    inline GpSocketMessage&     SetFlags            (const int aFalgs) noexcept;

private:
    GpSocketAddr                                            iAddr;
    std::array<std::byte, GP_NETWORK_UDP_MAX_PACKET_SIZE>   iDataBuffer;
    size_t                                                  iDataPayloadSize    = 0;
    std::array<std::byte, GP_NETWORK_CONTROL_INFO_SIZE>     iControlBuffer;
    int                                                     iFlags              = 0;
};

const GpSocketAddr& GpSocketMessage::Addr (void) const noexcept
{
    return iAddr;
}

GpSocketAddr&   GpSocketMessage::Addr (void) noexcept
{
    return iAddr;
}

GpSocketMessage&    GpSocketMessage::SetAddr (const GpSocketAddr& aAddr) noexcept
{
    iAddr = aAddr;

    return *this;
}

GpSpanPtrByteR  GpSocketMessage::DataPayload (void) const noexcept
{
    return GpSpanPtrByteR(iDataBuffer.data(), iDataPayloadSize);
}

GpSpanPtrByteRW GpSocketMessage::DataPayload (void) noexcept
{
    return GpSpanPtrByteRW(iDataBuffer.data(), iDataPayloadSize);
}

GpSpanPtrByteRW GpSocketMessage::DataStorage (void) noexcept
{
    return GpSpanPtrByteRW(iDataBuffer.data(), iDataBuffer.size());
}

GpSocketMessage&    GpSocketMessage::SetDataPayloadSize (const size_t aSize)
{
    THROW_COND_GP
    (
        aSize <= iDataBuffer.size(),
        [&]()
        {
            return fmt::format
            (
                "Data payload size is out of range. Max size {}, required size {}",
                iDataBuffer.size(),
                aSize
            );
        }
    );

    iDataPayloadSize = aSize;

    return *this;
}

GpSpanPtrByteR  GpSocketMessage::ControlPayload (void) const noexcept
{
    return GpSpanPtrByteR(iControlBuffer.data(), iControlBuffer.size());
}

GpSpanPtrByteRW GpSocketMessage::ControlPayload (void) noexcept
{
    return GpSpanPtrByteRW(iControlBuffer.data(), iControlBuffer.size());
}

GpSpanPtrByteRW GpSocketMessage::ControlStorage (void) noexcept
{
    return GpSpanPtrByteRW(iControlBuffer.data(), iControlBuffer.size());
}

int GpSocketMessage::Flags (void) const noexcept
{
    return iFlags;
}

GpSocketMessage&    GpSocketMessage::SetFlags (const int aFalgs) noexcept
{
    iFlags = aFalgs;

    return *this;
}

}// namespace GPlatform

#include "GpSocketUDP.hpp"

#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>

namespace GPlatform {

GpSocketUDP::~GpSocketUDP (void) noexcept
{
    [[maybe_unused]] int d = 0;
}

void    GpSocketUDP::Connect (const GpSocketAddr& aAddr)
{
    try
    {
        Create(aAddr.IPv());

        const int res = connect(Id(), aAddr.Raw(), aAddr.RawSize());

        THROW_COND_GP
        (
            res == 0,
            [](){return std::u8string(GpErrno::SGetAndClear());}
        );

        SetAddrLocal(GpSocketAddr::SLocalFromSocketId(Id()));
        SetAddrRemote(GpSocketAddr::SRemoteFromSocketId(Id()));
    } catch (...)
    {
        Close();
        throw;
    }
}

std::optional<size_t>   GpSocketUDP::ReadFrom
(
    GpByteWriter& aWriter,
    GpSocketAddr& aFromAddrOut
)
{
    constexpr const size_t maxPayloadSize = 2048;//TODO: move to socket settings (or get serttings from OS)

    if (const size_t sizeLeft = aWriter.SizeLeft(); sizeLeft < maxPayloadSize) [[unlikely]]
    {
        aWriter.ReserveNext(maxPayloadSize - sizeLeft);
    }

    GpSpanPtrByteRW dataStoragePtr  = aWriter.StoragePtr();
    socklen_t       fromAddrSize    = aFromAddrOut.RawSize();

    const ssize_t rcvSize = recvfrom
    (
        Id(),
        dataStoragePtr.Ptr(),
        dataStoragePtr.Count(),
        0,
        aFromAddrOut.Raw(),
        &fromAddrSize
    );

    if (rcvSize > 0) [[likely]]
    {
        aWriter.Offset(static_cast<size_t>(rcvSize));
        return static_cast<size_t>(rcvSize);
    } else if (rcvSize < 0) [[unlikely]]
    {
GP_WARNING_PUSH()
GP_WARNING_DISABLE(unknown-warning-option)
GP_WARNING_DISABLE(logical-op)
        if (   (errno == EAGAIN)
            || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return std::nullopt;
        } else
        {
            THROW_GP(GpErrno::SGetAndClear());
        }
    } else// rcvSize == 0
    {
        return 0;
    }
}

std::optional<size_t>   GpSocketUDP::Read (GpByteWriter& aWriter)
{
    constexpr const size_t maxPayloadSize = 2048;//TODO: move to socket settings (or get serttings from OS)

    if (const size_t sizeLeft = aWriter.SizeLeft(); sizeLeft < maxPayloadSize) [[unlikely]]
    {
        aWriter.ReserveNext(maxPayloadSize - sizeLeft);
    }

    GpSpanPtrByteRW dataStoragePtr  = aWriter.StoragePtr();

    const ssize_t rcvSize = recv
    (
        Id(),
        dataStoragePtr.Ptr(),
        dataStoragePtr.Count(),
        0
    );

    if (rcvSize > 0) [[likely]]
    {
        aWriter.Offset(static_cast<size_t>(rcvSize));
        return static_cast<size_t>(rcvSize);
    } else if (rcvSize < 0) [[unlikely]]
    {
        GP_WARNING_PUSH()
        GP_WARNING_DISABLE(unknown-warning-option)
        GP_WARNING_DISABLE(logical-op)
        if (   (errno == EAGAIN)
            || (errno == EWOULDBLOCK))
            GP_WARNING_POP()
            {
            return std::nullopt;
            } else
        {
            THROW_GP(GpErrno::SGetAndClear());
        }
    } else// rcvSize == 0
    {
        return 0;
    }
}

bool    GpSocketUDP::WriteTo
(
    GpSpanPtrByteR      aData,
    const GpSocketAddr& aToAddr
)
{
    const ssize_t sendSize = sendto
    (
        Id(),
        aData.Ptr(),
        aData.Count(),
        0,
        aToAddr.Raw(),
        aToAddr.RawSize()
    );

    if (sendSize < 0) [[unlikely]]
    {
GP_WARNING_PUSH()
GP_WARNING_DISABLE(unknown-warning-option)
GP_WARNING_DISABLE(logical-op)
        if (   (errno == EAGAIN)
            || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return false;
        } else
        {
            THROW_GP(GpErrno::SGetAndClear());
        }
    }

    return true;
}

bool    GpSocketUDP::Write (GpSpanPtrByteR aData)
{
    const ssize_t sendSize = send
    (
        Id(),
        aData.Ptr(),
        aData.Count(),
        0
    );

    if (sendSize < 0) [[unlikely]]
    {
        GP_WARNING_PUSH()
        GP_WARNING_DISABLE(unknown-warning-option)
        GP_WARNING_DISABLE(logical-op)
        if (   (errno == EAGAIN)
            || (errno == EWOULDBLOCK))
        GP_WARNING_POP()
        {
            return false;
        } else
        {
            THROW_GP(GpErrno::SGetAndClear());
        }
    }

    return true;
}

}// namespace GPlatform

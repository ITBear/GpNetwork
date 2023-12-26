#include "GpSocketUDP.hpp"

#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>

namespace GPlatform {

GpSocketUDP::~GpSocketUDP (void) noexcept
{
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
    if (const size_t sizeLeft = aWriter.SizeLeft(); sizeLeft < size_t(GP_NETWORK_UDP_MAX_PACKET_SIZE)) [[unlikely]]
    {
        aWriter.ReserveNext(size_t(GP_NETWORK_UDP_MAX_PACKET_SIZE) - sizeLeft);
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
    if (const size_t sizeLeft = aWriter.SizeLeft(); sizeLeft < size_t(GP_NETWORK_UDP_MAX_PACKET_SIZE)) [[unlikely]]
    {
        aWriter.ReserveNext(size_t(GP_NETWORK_UDP_MAX_PACKET_SIZE) - sizeLeft);
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

std::optional<size_t>   GpSocketUDP::RecvMsg (GpSocketMessage& aMessageOut)
{
    struct msghdr               msg;
    std::array<struct iovec, 1> iov;

    memset(&msg, 0, sizeof(msg));

    iov[0].iov_base     = aMessageOut.DataStorage().Ptr();
    iov[0].iov_len      = aMessageOut.DataStorage().Count();
    msg.msg_iov         = iov.data();
    msg.msg_iovlen      = iov.size();
    msg.msg_name        = aMessageOut.Addr().Raw();
    msg.msg_namelen     = aMessageOut.Addr().RawSize();
    msg.msg_control     = aMessageOut.ControlPayload().Ptr();
    msg.msg_controllen  = aMessageOut.ControlPayload().Count();

    const ssize_t rcvSize = recvmsg(Id(), &msg, 0);

    if (rcvSize > 0) [[likely]]
    {
        aMessageOut
            .SetDataPayloadSize(static_cast<size_t>(rcvSize))
            .SetFlags(msg.msg_flags);

        return aMessageOut.DataPayload().Count();
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

bool    GpSocketUDP::SendMsg (const GpSocketMessage& aMessage)
{
    struct msghdr               msg;
    std::array<struct iovec, 1> iov;

    memset(&msg, 0, sizeof(msg));

    iov[0].iov_base     = const_cast<std::byte*>(aMessage.DataPayload().Ptr());
    iov[0].iov_len      = aMessage.DataPayload().Count();
    msg.msg_iov         = iov.data();
    msg.msg_iovlen      = iov.size();
    msg.msg_name        = const_cast<sockaddr*>(aMessage.Addr().Raw());
    msg.msg_namelen     = aMessage.Addr().RawSize();
    msg.msg_control     = const_cast<std::byte*>(aMessage.ControlPayload().Ptr());
    msg.msg_controllen  = aMessage.ControlPayload().Count();
    msg.msg_flags       = aMessage.Flags();

    const ssize_t sendSize = sendmsg
    (
        Id(),
        &msg,
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

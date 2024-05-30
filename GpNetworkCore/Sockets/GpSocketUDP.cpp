#include "GpSocketUDP.hpp"

#include "../GpNetworkErrors.hpp"

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
        SCheckResOrThrow(res, {});

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
    if (const size_t sizeLeft = aWriter.SizeLeft(); sizeLeft < size_t(GP_NETWORK_UDP_PACKET_MAX_SIZE)) [[unlikely]]
    {
        aWriter.ReserveNext(size_t(GP_NETWORK_UDP_PACKET_MAX_SIZE) - sizeLeft);
    }

    GpSpanByteRW    dataStoragePtr  = aWriter.StoragePtr();
    socklen_t       fromAddrSize    = aFromAddrOut.RawSize();

    const ssize_t rcvSize = recvfrom
    (
        Id(),
        reinterpret_cast<char*>(dataStoragePtr.Ptr()),
        NumOps::SConvert<recv_size_t>(dataStoragePtr.Count()),
        0,
        aFromAddrOut.Raw(),
        &fromAddrSize
    );

    if (rcvSize > 0) [[likely]]
    {
        aWriter.OffsetAdd(static_cast<size_t>(rcvSize));
        return static_cast<size_t>(rcvSize);
    } else if (rcvSize < 0) [[unlikely]]
    {
GP_WARNING_PUSH()
#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(unknown-warning-option)
    GP_WARNING_DISABLE(logical-op)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
        if (   (errno == EAGAIN)
            || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return std::nullopt;
        }

        SCheckResOrThrow(-1, {});
    }

    return 0;
}

std::optional<size_t>   GpSocketUDP::Read (GpByteWriter& aWriter)
{
    if (const size_t sizeLeft = aWriter.SizeLeft(); sizeLeft < size_t(GP_NETWORK_UDP_PACKET_MAX_SIZE)) [[unlikely]]
    {
        aWriter.ReserveNext(size_t(GP_NETWORK_UDP_PACKET_MAX_SIZE) - sizeLeft);
    }

    GpSpanByteRW dataStoragePtr = aWriter.StoragePtr();

    const ssize_t rcvSize = recv
    (
        Id(),
        reinterpret_cast<char*>(dataStoragePtr.Ptr()),
        NumOps::SConvert<recv_size_t>(dataStoragePtr.Count()),
        0
    );

    if (rcvSize > 0) [[likely]]
    {
        aWriter.OffsetAdd(static_cast<size_t>(rcvSize));
        return static_cast<size_t>(rcvSize);
    } else if (rcvSize < 0) [[unlikely]]
    {
GP_WARNING_PUSH()
#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(unknown-warning-option)
    GP_WARNING_DISABLE(logical-op)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
        if (   (errno == EAGAIN)
            || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return std::nullopt;
        }

        SCheckResOrThrow(-1, {});
    }

    return 0;
}

std::optional<size_t>   GpSocketUDP::RecvMsg (GpSocketMessageUDP& aMessageOut)
{
#if defined(GP_POSIX)// ----------------------------------- #if defined(GP_POSIX) ---------------------------------
    std::array<struct iovec, 1> iov;
    iov[0].iov_base = aMessageOut.Data_().Ptr();
    iov[0].iov_len  = aMessageOut.Data_().Count();

    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    msg.msg_iov         = std::data(iov);
    msg.msg_iovlen      = std::size(iov);
    msg.msg_name        = aMessageOut.Addr().Raw();
    msg.msg_namelen     = aMessageOut.Addr().RawSize();
    msg.msg_control     = aMessageOut.ControlBuffer().Ptr();
    msg.msg_controllen  = aMessageOut.ControlBuffer().Count();

    const ssize_t rcvSize = recvmsg(Id(), &msg, 0);

    if (rcvSize > 0) [[likely]]
    {
        aMessageOut
            .SetDataUseSize(static_cast<size_t>(rcvSize))
            .SetFlags(msg.msg_flags);

        return aMessageOut.DataUseSize();
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
        }

        SCheckResOrThrow(-1, {});
    }

    return 0;
#elif defined(GP_OS_WINDOWS)// ----------------------------------- #elif defined(GP_OS_WINDOWS) ---------------------------------
    std::array<WSABUF, 1> iov;
    iov[0].buf  = reinterpret_cast<char*>(aMessageOut.Data_().Ptr());
    iov[0].len  = NumOps::SConvert<decltype(WSABUF::len)>(aMessageOut.Data_().Count());

    WSAMSG msg;
    memset(&msg, 0, sizeof(msg));

    msg.lpBuffers       = std::data(iov);
    msg.dwBufferCount   = 1;
    msg.name            = aMessageOut.Addr().Raw();
    msg.namelen         = aMessageOut.Addr().RawSize();
    msg.Control.buf     = reinterpret_cast<char*>(aMessageOut.ControlBuffer().Ptr());
    msg.Control.len     = NumOps::SConvert<decltype(WSABUF::len)>(aMessageOut.ControlBuffer().Count());

    if (iWSARecvMsg == nullptr) [[unlikely]]
    {
        iWSARecvMsg = SGetWSARecvMsgPtr(*this);
    }

    DWORD rcvSize = 0;

    const int wsaRes = iWSARecvMsg(Id(), &msg, &rcvSize, nullptr, nullptr);

    if (wsaRes < 0) [[unlikely]]
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
        {
            return 0;
        }

        THROW_GP(GpNetworkErrors::SGetLastError());
    }

    if (rcvSize == 0) [[unlikely]]
    {
        return 0;
    }

    aMessageOut
        .SetDataUseSize(static_cast<size_t>(rcvSize))
        .SetFlags(msg.dwFlags);

    return aMessageOut.DataUseSize();
#endif
}

bool    GpSocketUDP::WriteTo
(
    GpSpanByteR         aData,
    const GpSocketAddr& aToAddr
)
{
    const send_size_t dataSize = NumOps::SConvert<send_size_t>(aData.Count());

    const ssize_t sendSize = sendto
    (
        Id(),
        reinterpret_cast<char*>(const_cast<std::byte*>(aData.Ptr())),
        dataSize,
        0,
        aToAddr.Raw(),
        aToAddr.RawSize()
    );

    if (sendSize < 0) [[unlikely]]
    {
GP_WARNING_PUSH()
#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(unknown-warning-option)
    GP_WARNING_DISABLE(logical-op)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
        if (   (errno == EAGAIN)
            || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return false;
        }

        SCheckResOrThrow(-1, {});
    }

    THROW_COND_GP
    (
        dataSize == NumOps::SConvert<decltype(dataSize)>(sendSize),
        "dataSize != sendSize"
    );

    return true;
}

bool    GpSocketUDP::Write (GpSpanByteR aData)
{
    const send_size_t dataSize = NumOps::SConvert<send_size_t>(aData.Count());

    const ssize_t sendSize = send
    (
        Id(),
        reinterpret_cast<char*>(const_cast<std::byte*>(aData.Ptr())),
        dataSize,
        0
    );

    if (sendSize < 0) [[unlikely]]
    {
GP_WARNING_PUSH()
#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(unknown-warning-option)
    GP_WARNING_DISABLE(logical-op)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
        if (   (errno == EAGAIN)
            || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return false;
        }

        SCheckResOrThrow(-1, {});
    }

    THROW_COND_GP
    (
        dataSize == NumOps::SConvert<decltype(dataSize)>(sendSize),
        "dataSize != sendSize"
    );

    return true;
}

bool    GpSocketUDP::SendMsg (const GpSocketMessageUDP& aMessage)
{
#if defined(GP_POSIX)// ----------------------------------- #if defined(GP_POSIX) ---------------------------------
    struct msghdr               msg;
    std::array<struct iovec, 1> iov;

    memset(&msg, 0, sizeof(msg));

    iov[0].iov_base     = const_cast<std::byte*>(aMessage.DataUseOnly().Ptr());
    iov[0].iov_len      = aMessage.DataUseOnly().Count();
    msg.msg_iov         = std::data(iov);
    msg.msg_iovlen      = std::size(iov);
    msg.msg_name        = const_cast<sockaddr*>(aMessage.Addr().Raw());
    msg.msg_namelen     = aMessage.Addr().RawSize();
    msg.msg_control     = const_cast<std::byte*>(aMessage.ControlBufferUseOnly().Ptr());
    msg.msg_controllen  = aMessage.ControlBufferUseOnly().Count();
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
        }

        SCheckResOrThrow(-1, {});
    }

    return true;
#elif defined(GP_OS_WINDOWS)// ----------------------------------- #elif defined(GP_OS_WINDOWS) ---------------------------------
    std::array<WSABUF, 1> iov;
    iov[0].buf  = reinterpret_cast<char*>(const_cast<std::byte*>(aMessage.DataUseOnly().Ptr()));
    iov[0].len  = NumOps::SConvert<decltype(WSABUF::len)>(aMessage.DataUseOnly().Count());

    WSAMSG msg;
    memset(&msg, 0, sizeof(msg));

    msg.lpBuffers       = std::data(iov);
    msg.dwBufferCount   = 1;
    msg.name            = const_cast<sockaddr*>(aMessage.Addr().Raw());
    msg.namelen         = aMessage.Addr().RawSize();
    msg.Control.buf     = reinterpret_cast<char*>(const_cast<std::byte*>(aMessage.ControlBufferUseOnly().Ptr()));
    msg.Control.len     = NumOps::SConvert<decltype(WSABUF::len)>(aMessage.ControlBufferUseOnly().Count());

    if (iWSASendMsg == nullptr) [[unlikely]]
    {
        iWSASendMsg = SGetWSASendMsgPtr(*this);
    }

    DWORD sendSize = 0;

    const int wsaRes = iWSASendMsg(Id(), &msg, 0, &sendSize, nullptr, nullptr);

    if (wsaRes < 0) [[unlikely]]
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
        {
            return false;
        }

        THROW_GP(GpNetworkErrors::SGetLastError());
    }

    return true;
#endif
}

#if defined(GP_OS_WINDOWS)// ----------------------------------- #if defined(GP_OS_WINDOWS) ---------------------------------

LPFN_WSARECVMSG GpSocketUDP::SGetWSARecvMsgPtr (const GpSocketUDP& aSocket)
{
    DWORD           dwBytesReturned = 0;
    GUID            guidWSARecvMsg  = WSAID_WSARECVMSG;
    LPFN_WSARECVMSG lpfnWSARecvMsg  = nullptr;

    const int res = WSAIoctl
    (
        aSocket.Id(),
        SIO_GET_EXTENSION_FUNCTION_POINTER,
        &guidWSARecvMsg,
        sizeof(guidWSARecvMsg),
        &lpfnWSARecvMsg,
        sizeof(lpfnWSARecvMsg),
        &dwBytesReturned,
        nullptr,
        nullptr
    );

    SCheckResOrThrow(res, {});

    return lpfnWSARecvMsg;
}

LPFN_WSASENDMSG GpSocketUDP::SGetWSASendMsgPtr (const GpSocketUDP& aSocket)
{
    DWORD           dwBytesReturned = 0;
    GUID            guidWSASendMsg  = WSAID_WSASENDMSG;
    LPFN_WSASENDMSG lpfnWSASendMsg  = nullptr;

    const int res = WSAIoctl
    (
        aSocket.Id(),
        SIO_GET_EXTENSION_FUNCTION_POINTER,
        &guidWSASendMsg,
        sizeof(guidWSASendMsg),
        &lpfnWSASendMsg,
        sizeof(lpfnWSASendMsg),
        &dwBytesReturned,
        nullptr,
        nullptr
    );

    SCheckResOrThrow(res, {});

    return lpfnWSASendMsg;
}

#endif// #if defined(GP_OS_WINDOWS)// ----------------------------------- #endif// #if defined(GP_OS_WINDOWS) ---------------------------------

}// namespace GPlatform

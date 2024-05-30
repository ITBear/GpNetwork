#include "GpSocketUtils.hpp"

namespace GPlatform {

const struct in_pktinfo*    GpSocketUtils::SGetPktInfo
(
    GpSpanByteR                 aControlBuffer,
    const GpSocketIPv::EnumT    aIPv
)
{
    const struct in_pktinfo*    pktinfo     = nullptr;
    const int                   ipProtoVer  = aIPv == GpSocketIPv::IPv4 ? IPPROTO_IP : IPPROTO_IPV6;

#if defined(GP_POSIX)
    struct msghdr msg;
    std::memset(&msg, 0, sizeof(msg));

    msg.msg_control     = const_cast<std::byte*>(aControlBuffer.Ptr());
    msg.msg_controllen  = aControlBuffer.Count();

    struct cmsghdr* cmsgHdr = nullptr;
    for (cmsgHdr = CMSG_FIRSTHDR(&msg); cmsgHdr != nullptr; cmsgHdr = CMSG_NXTHDR(&msg, cmsgHdr))
    {
        if (   (cmsgHdr->cmsg_level == ipProtoVer)
            && (cmsgHdr->cmsg_type == IP_PKTINFO))
        {
            pktinfo = reinterpret_cast<const struct in_pktinfo*>(CMSG_DATA(cmsgHdr));
            break;
        }
    }
#elif defined(GP_OS_WINDOWS)
    WSAMSG msg;
    std::memset(&msg, 0, sizeof(msg));

    msg.Control.buf = reinterpret_cast<char*>(const_cast<std::byte*>(aControlBuffer.Ptr()));
    msg.Control.len = NumOps::SConvert<decltype(WSABUF::len)>(aControlBuffer.Count());

    THROW_COND_GP
    (
        msg.Control.len >= WSA_CMSG_SPACE(sizeof(struct in_pktinfo)),
        [&]()
        {
            return fmt::format
            (
                "Control payload size {} muse be >= {} for use in_pktinfo",
                msg.Control.len,
                WSA_CMSG_SPACE(sizeof(struct in_pktinfo))
            );
        }
    );

    WSACMSGHDR* cmsgHdr = nullptr;
    for (cmsgHdr = WSA_CMSG_FIRSTHDR(&msg); cmsgHdr != nullptr; cmsgHdr = WSA_CMSG_NXTHDR(&msg, cmsgHdr))
    {
        if (   (cmsgHdr->cmsg_level == ipProtoVer)
            && (cmsgHdr->cmsg_type == IP_PKTINFO))
        {
            pktinfo = reinterpret_cast<const struct in_pktinfo*>(WSA_CMSG_DATA(cmsgHdr));
            break;
        }
    }
#else
#   error Unsupported OS
#endif

    return pktinfo;
}

size_t  GpSocketUtils::SSetPktInfo
(
    GpSpanByteRW                aControlBuffer,
    const struct in_pktinfo*    aPktInfo,
    const GpSocketIPv::EnumT    aIPv
)
{
    THROW_COND_GP
    (
        aPktInfo != nullptr,
        "aPktInfo is null"
    );

#if defined(GP_POSIX)
    struct msghdr msg;
    std::memset(&msg, 0, sizeof(msg));

    msg.msg_control     = const_cast<std::byte*>(aControlBuffer.Ptr());
    msg.msg_controllen  = aControlBuffer.Count();

    struct cmsghdr* cmsgHdr = CMSG_FIRSTHDR(&msg);

    THROW_COND_GP
    (
        cmsgHdr != nullptr,
        "CMSG_FIRSTHDR return null"
    );

    cmsgHdr->cmsg_level = aIPv == GpSocketIPv::IPv4 ? IPPROTO_IP : IPPROTO_IPV6;
    cmsgHdr->cmsg_type  = IP_PKTINFO;
    cmsgHdr->cmsg_len   = CMSG_LEN(sizeof(struct in_pktinfo));

    THROW_COND_GP
    (
        aControlBuffer.Count() >= CMSG_LEN(sizeof(struct in_pktinfo)),
        [&]()
        {
            return fmt::format
            (
                "Control payload size {} muse be >= {} for use in_pktinfo",
                aControlBuffer.Count(),
                CMSG_SPACE(sizeof(struct in_pktinfo))
            );
        }
    );

    std::memcpy(CMSG_DATA(cmsgHdr), aPktInfo, sizeof(struct in_pktinfo));

    return msg.msg_controllen;
#elif defined(GP_OS_WINDOWS)
    WSAMSG msg;
    std::memset(&msg, 0, sizeof(msg));

    msg.Control.buf = reinterpret_cast<char*>(const_cast<std::byte*>(aControlBuffer.Ptr()));
    msg.Control.len = WSA_CMSG_SPACE(sizeof(struct in_pktinfo));

    THROW_COND_GP
    (
        aControlBuffer.Count() >= WSA_CMSG_SPACE(sizeof(struct in_pktinfo)),
        [&]()
        {
            return fmt::format
            (
                "Control payload size {} muse be >= {} for use in_pktinfo",
                aControlBuffer.Count(),
                WSA_CMSG_SPACE(sizeof(struct in_pktinfo))
            );
        }
    );

    WSACMSGHDR* cmsgHdr = WSA_CMSG_FIRSTHDR(&msg);

    THROW_COND_GP
    (
        cmsgHdr != nullptr,
        "WSA_CMSG_FIRSTHDR return null"
    );

    cmsgHdr->cmsg_level = aIPv == GpSocketIPv::IPv4 ? IPPROTO_IP : IPPROTO_IPV6;
    cmsgHdr->cmsg_type  = IP_PKTINFO;
    cmsgHdr->cmsg_len   = WSA_CMSG_LEN(sizeof(struct in_pktinfo));

    std::memcpy(WSA_CMSG_DATA(cmsgHdr), aPktInfo, sizeof(struct in_pktinfo));

    return msg.Control.len;
#else
#   error Unsupported OS
#endif
}

}// namespace GPlatform

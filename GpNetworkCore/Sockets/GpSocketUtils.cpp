#include "GpSocketUtils.hpp"

#include <sys/socket.h>

namespace GPlatform {

const struct in_pktinfo*    GpSocketUtils::SGetPktInfo
(
    GpSpanPtrByteR              aControlPayload,
    const GpSocketIPv::EnumT    aIPv
)
{
    const struct in_pktinfo*    pktinfo     = nullptr;
    const int                   ipProtoVer  = aIPv == GpSocketIPv::IPv4 ? IPPROTO_IP : IPPROTO_IPV6;

    struct msghdr msgRq;
    msgRq.msg_control       = const_cast<std::byte*>(aControlPayload.Ptr());
    msgRq.msg_controllen    = aControlPayload.Count();

    struct cmsghdr* cmsgHdrRq = nullptr;
    for (cmsgHdrRq = CMSG_FIRSTHDR(&msgRq); cmsgHdrRq != nullptr; cmsgHdrRq = CMSG_NXTHDR(&msgRq, cmsgHdrRq))
    {
        if (   (cmsgHdrRq->cmsg_level == ipProtoVer)
            && (cmsgHdrRq->cmsg_type == IP_PKTINFO))
        {
            pktinfo = reinterpret_cast<const struct in_pktinfo*>(CMSG_DATA(cmsgHdrRq));
            break;
        }
    }

    return pktinfo;
}

void    GpSocketUtils::SSetPktInfo
(
    GpSpanPtrByteRW             aControlPayload,
    const struct in_pktinfo*    aPktInfo,
    const GpSocketIPv::EnumT    aIPv
)
{
    THROW_COND_GP
    (
        aPktInfo != nullptr,
        "aPktInfo is null"
    );

    struct msghdr msgRs;
    msgRs.msg_control       = const_cast<std::byte*>(aControlPayload.Ptr());
    msgRs.msg_controllen    = aControlPayload.Count();

    struct cmsghdr* cmsgHdrRs = CMSG_FIRSTHDR(&msgRs);

    THROW_COND_GP
    (
        cmsgHdrRs != nullptr,
        "CMSG_FIRSTHDR return null"
    );

    cmsgHdrRs->cmsg_level   = aIPv == GpSocketIPv::IPv4 ? IPPROTO_IP : IPPROTO_IPV6;
    cmsgHdrRs->cmsg_type    = IP_PKTINFO;
    cmsgHdrRs->cmsg_len     = CMSG_LEN(sizeof(struct in_pktinfo));

    std::memcpy(CMSG_DATA(cmsgHdrRs), aPktInfo, sizeof(struct in_pktinfo));
}

}// namespace GPlatform

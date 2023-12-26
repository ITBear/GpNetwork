#pragma once

#include "../GpNetworkCore_global.hpp"
#include "GpSocketIPv.hpp"

#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>

struct in_pktinfo;

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketUtils
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSocketUtils)

public:
    static const struct in_pktinfo* SGetPktInfo     (GpSpanPtrByteR             aControlPayload,
                                                     const GpSocketIPv::EnumT   aIPv);
    static void                     SSetPktInfo     (GpSpanPtrByteRW            aControlPayload,
                                                     const struct in_pktinfo*   aPktInfo,
                                                     const GpSocketIPv::EnumT   aIPv);
};

}// namespace GPlatform

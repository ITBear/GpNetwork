#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroImportExport.hpp>

#if defined(GP_NETWORK_CORE_LIBRARY)
    #define GP_NETWORK_CORE_API GP_DECL_EXPORT
#else
    #define GP_NETWORK_CORE_API GP_DECL_IMPORT
#endif

#define GP_NETWORK_UDP_MAX_PACKET_SIZE  2048
#define GP_NETWORK_CONTROL_INFO_SIZE    CMSG_SPACE(sizeof(struct in_pktinfo))

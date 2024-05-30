#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroImportExport.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

#if defined(GP_POSIX)//-----------------------------------------------------
#   include <arpa/inet.h>
#   include <fcntl.h>
#   include <netinet/in.h>
#   include <netinet/in.h>
#   include <netinet/tcp.h>
#   include <sys/socket.h>
#   include <sys/types.h>
#   include <unistd.h>

    using recv_size_t   = size_t;
    using send_size_t   = size_t;

    using GpSocketId    = int;
    constexpr GpSocketId GpSocketId_Default (void) {return GpSocketId(-1);}

#   define GP_NETWORK_UDP_PACKET_MAX_SIZE   1500
#   define GP_NETWORK_CONTROL_INFO_MAX_SIZE 256 // CMSG_SPACE(sizeof(struct in_pktinfo))
#endif// #if defined(GP_POSIX)

#if defined(GP_OS_WINDOWS)//-----------------------------------------------------
    GP_WARNING_PUSH()

#   if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
        GP_WARNING_DISABLE(expansion-to-defined)
        GP_WARNING_DISABLE(conversion)
#   endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#   define NOMINMAX

#   include <winsock2.h>
#   include <mswsock.h>
#   include <ws2ipdef.h>
#   include <ws2tcpip.h>

    GP_WARNING_POP()

    using socklen_t     = int;
    using recv_size_t   = int;
    using send_size_t   = int;
    using sa_family_t   = decltype(sockaddr_storage::ss_family);

    using GpSocketId    = SOCKET;
    constexpr GpSocketId GpSocketId_Default (void) {return GpSocketId(INVALID_SOCKET);}

#   define GP_NETWORK_UDP_PACKET_MAX_SIZE   1500
#   define GP_NETWORK_CONTROL_INFO_MAX_SIZE 256 // WSA_CMSG_SPACE(sizeof(struct in_pktinfo))

#endif// #if defined(GP_OS_WINDOWS)

#if defined(GP_NETWORK_CORE_LIBRARY)
    #define GP_NETWORK_CORE_API GP_DECL_EXPORT
#else
    #define GP_NETWORK_CORE_API GP_DECL_IMPORT
#endif

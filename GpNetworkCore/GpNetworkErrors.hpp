#pragma once

#include <GpNetwork/GpNetworkCore/GpNetworkCore_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#if defined(GP_POSIX)
#   include <GpCore2/GpUtils/Other/GpErrno.hpp>
#endif// #if defined(GP_POSIX)

namespace GPlatform {

class GP_NETWORK_CORE_API GpNetworkErrors
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpNetworkErrors)

public:
#if defined(GP_POSIX)
    static std::string      SGetLastError   (void) {return std::string(GpErrno::SGetAndClear());}
    static int              SErrno          (void) {return errno;}
    static bool             SIsWouldBlock   (const int aErrno) {return aErrno == EWOULDBLOCK;}
    static bool             SConnInProgress (const int aErrno) {return aErrno == EINPROGRESS;}
#elif defined(GP_OS_WINDOWS)
    static std::string      SGetLastError   (void);
    static int              SErrno          (void) {return WSAGetLastError();}
    static bool             SIsWouldBlock   (const int aErrno) {return aErrno == WSAEWOULDBLOCK;}
    static bool             SConnInProgress (const int aErrno) {return (aErrno == WSAEWOULDBLOCK) || (aErrno == EINPROGRESS);}
#endif// #if defined(GP_OS_WINDOWS)
};

}// namespace GPlatform

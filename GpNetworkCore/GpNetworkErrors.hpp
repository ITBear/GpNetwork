#pragma once

#include "GpNetworkCore_global.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <string>

#if defined(GP_POSIX)
#   include <GpCore2/GpUtils/Other/GpErrno.hpp>
#endif// #if defined(GP_POSIX)

namespace GPlatform {

class GP_NETWORK_CORE_API GpNetworkErrors
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpNetworkErrors)

public:
#if defined(GP_POSIX)
    static std::string      SGetLastError       (void) {return std::string(GpErrno::SGetAndClear());}
#elif defined(GP_OS_WINDOWS)
    static std::string      SGetLastError       (void) {return SGetLastErrorWSA();}
    static std::string      SGetLastErrorWSA    (void);
#endif// #if defined(GP_OS_WINDOWS)
};

}// namespace GPlatform

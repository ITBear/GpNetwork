#include "GpNetworkErrors.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptions.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.cpp>

#if defined(GP_OS_WINDOWS)
#   include <GpCore2/Config/IncludeExt/windows.hpp>
#endif// #if defined(GP_OS_WINDOWS)

namespace GPlatform {

#if defined(GP_OS_WINDOWS)// ----------------------------------- #if defined(GP_OS_WINDOWS) ---------------------------------

std::string GpNetworkErrors::SGetLastErrorWSA (void)
{
    char*       msgPtr      = nullptr;
    const auto  errorCode   = WSAGetLastError();

    const DWORD size = FormatMessageA
    (
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msgPtr,
        0,
        nullptr
    );

    THROW_COND_GP
    (
        size != 0,
        []()
        {
            return "[GpNetworkErrors::SGetLastErrorWSA]: Failed with error: " + GpErrno::SWinGetAndClear();
        }
    );

    std::string resMsgStr(msgPtr, size);

    LocalFree(msgPtr);

    return resMsgStr;
}

#endif// #if defined(GP_OS_WINDOWS)// ----------------------------------- #endif// #if defined(GP_OS_WINDOWS) ---------------------------------

}// namespace GPlatform

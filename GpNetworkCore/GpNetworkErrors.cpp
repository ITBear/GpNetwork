#include <GpNetwork/GpNetworkCore/GpNetworkErrors.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptions.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>

#if defined(GP_OS_WINDOWS)
#   include <GpCore2/Config/IncludeExt/windows.hpp>
#endif// #if defined(GP_OS_WINDOWS)

namespace GPlatform {

#if defined(GP_OS_WINDOWS)// ----------------------------------- #if defined(GP_OS_WINDOWS) ---------------------------------

std::string GpNetworkErrors::SGetLastError (void)
{
    char*       msgPtr      = nullptr;
    const auto  errorCode   = WSAGetLastError();

    const DWORD msgSize = FormatMessageA
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
        msgSize != 0,
        []()
        {
            return "[GpNetworkErrors::SGetLastError]: Failed with error: " + GpErrno::SWinGetAndClear();
        }
    );

    std::string_view msg{msgPtr, msgSize};
    const auto idx = msg.find_first_of("\r\n");

    std::string msgStr(idx == std::string_view::npos ? msg : msg.substr(0, idx));

    LocalFree(msgPtr);

    return msgStr;
}

#endif// #if defined(GP_OS_WINDOWS)// ----------------------------------- #endif// #if defined(GP_OS_WINDOWS) ---------------------------------

}// namespace GPlatform

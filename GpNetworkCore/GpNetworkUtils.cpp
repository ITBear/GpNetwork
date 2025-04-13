#include <GpNetwork/GpNetworkCore/GpNetworkUtils.hpp>
#include <GpNetwork/GpNetworkCore/GpNetworkCore_global.hpp>
#include <GpNetwork/GpNetworkCore/GpNetworkErrors.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

namespace GPlatform {

void    GpNetworkUtils::SInitGlobal (void)
{
#if defined(GP_OS_WINDOWS)
    WSADATA wsaData;
    const int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    VERIFY
    (
        result == 0,
        []()
        {
            return GpNetworkErrors::SGetLastError();
        }
    );
#endif// #if defined(GP_OS_WINDOWS)
}

void    GpNetworkUtils::SFinishGlobal (void)
{
#if defined(GP_OS_WINDOWS)
    WSACleanup();
#endif// #if defined(GP_OS_WINDOWS)
}

}// namespace GPlatform

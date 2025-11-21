#include <GpNetwork/GpNetworkCore/Sockets/GpSocket.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>

namespace GPlatform {

GpSocket::~GpSocket (void) noexcept
{
    try
    {
        Close();
    } catch (const GpException& e)
    {
        GpOutUtils::S().Err("[GpSocket::~GpSocket]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpOutUtils::S().Err("[GpSocket::~GpSocket]: exception: "_sv + e.what());
    } catch (...)
    {
        GpOutUtils::S().Err("[GpSocket::~GpSocket]: unknown exception"_sv);
    }
}

}// namespace GPlatform

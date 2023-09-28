#include "GpSocket.hpp"

#include "../../../GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp"

namespace GPlatform {

GpSocket::~GpSocket (void) noexcept
{
    try
    {
        Close();
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpSocket::~GpSocket]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpSocket::~GpSocket]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpSocket::~GpSocket]: unknown exception"_sv);
    }
}

}//namespace GPlatform

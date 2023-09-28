#include "GpSocketFactoryTCP.hpp"
#include "GpSocketTCP.hpp"

namespace GPlatform {

GpSocket::SP GpSocketFactoryTCP::NewInstance (void) const
{
    return MakeSP<GpSocketTCP>(iFlags, iCloseMode);
}

}// namespace GPlatform {

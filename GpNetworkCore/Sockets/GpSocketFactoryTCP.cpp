#include "GpSocketFactoryTCP.hpp"
#include "GpSocketTCP.hpp"

namespace GPlatform {

GpSocket::SP GpSocketFactoryTCP::NewInstance (void) const
{
    return MakeSP<GpSocketTCP>(iFlags, iCloseMode);
}

void    GpSocketFactoryTCP::DestroyInstance (GpSocket& /*aSocket*/) const
{
    // NOP
}

}// namespace GPlatform {

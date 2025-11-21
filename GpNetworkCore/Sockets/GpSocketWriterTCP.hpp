#pragma once

#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketWriterTCP
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSocketWriterTCP)
    CLASS_DD(GpSocketWriterTCP)

public:
                GpSocketWriterTCP   (void) noexcept;
                ~GpSocketWriterTCP  (void) noexcept;

    void        AddToQueue          (GpByteArray aData);
    size_t      Write               (GpSocketTCP& aSocket);

private:
    std::queue<GpByteArray> iBuffers;
    GpSpanByteR             iCurrentBuffer;
};

}// namespace GPlatform

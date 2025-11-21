#include <GpNetwork/GpNetworkCore/Sockets/GpSocketWriterTCP.hpp>

namespace GPlatform {

GpSocketWriterTCP::GpSocketWriterTCP (void) noexcept
{
}

GpSocketWriterTCP::~GpSocketWriterTCP (void) noexcept
{
}

void    GpSocketWriterTCP::AddToQueue (GpByteArray aData)
{
    VERIFY
    (
        std::size(aData) > 0,
        "Data buffer is empty"
    );

    iBuffers.push(std::move(aData));
}

size_t  GpSocketWriterTCP::Write (GpSocketTCP& aSocket)
{
    size_t totalWrite = 0;

    while (!iBuffers.empty())
    {
        if (iCurrentBuffer.Empty())
        {
            iCurrentBuffer = iBuffers.front();
        }

        const size_t writeSize = aSocket.Write(iCurrentBuffer);

        if (writeSize == 0)// TCP buffer is full
        {
            return totalWrite;
        }

        totalWrite += writeSize;

        if (writeSize < iCurrentBuffer.SizeInBytes())
        {
            iCurrentBuffer.OffsetAdd(writeSize);
        } else
        {
            iBuffers.pop();
        }
    }

    return totalWrite;
}

}// namespace GPlatform

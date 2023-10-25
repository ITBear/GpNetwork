#include "GpSocketUDP.hpp"
#include "../../../GpCore2/GpUtils/Streams/GpByteWriter.hpp"
#include "../../../GpCore2/GpUtils/Streams/GpByteReader.hpp"

namespace GPlatform {

GpSocketUDP::~GpSocketUDP (void) noexcept
{
}

GpSocketUDP::SP GpSocketUDP::SFromID
(
    GpIOObjectId        /*aId*/,
    const CloseModeT    /*aCloseMode*/
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*GpSocketUDP::SP socketSP  = MakeSP<GpSocketUDP>(GpSocketFlags{}, aCloseMode);
    GpSocketUDP&    socket      = socketSP.V();

    socket.SetFromRaw(aId);

    return socketSP;*/
}

size_t  GpSocketUDP::Read
(
    GpByteWriter& /*aWriter*/,
    GpSocketAddr& /*aFromAddrOut*/
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*size_t totalRcvSize = 0;

    do
    {
        aWriter.ReserveNext(4096);//TODO: move to socket settings (or get serttings from OS)
        GpSpanPtrByteRW writerStoragePtr = aWriter.StoragePtr();

        socklen_t socklen = sizeof(cliaddr);
        const ssize_t rcvSize = recvfrom
        (
            Id(),
            writerStoragePtr.Ptr(),
            writerStoragePtr.Count(),
            0,
            aFromAddrOut.Raw(),
            &socklen
        );



        const ssize_t rcvSize = recv
        (
            Id(),
            writerStoragePtr.Ptr(),
            writerStoragePtr.Count(),
            0
        );

        if (rcvSize < 0) [[unlikely]]
        {
GP_WARNING_PUSH()
GP_WARNING_DISABLE(unknown-warning-option)
GP_WARNING_DISABLE(logical-op)
            if (   (errno == EAGAIN)
                || (errno == EWOULDBLOCK))
GP_WARNING_POP()
            {
                return totalRcvSize;
            } else
            {
                THROW_GP(GpErrno::SGetAndClear());
            }
        } else if (rcvSize == 0)
        {
            break;
        }

        totalRcvSize = NumOps::SAdd(totalRcvSize, size_t(rcvSize));
        aWriter.Offset(size_t(rcvSize));

        if (size_t(rcvSize) < writerStoragePtr.Count()) [[likely]]
        {
            break;
        }
    } while (true);

    return totalRcvSize;*/
}

size_t  GpSocketUDP::Write (GpSpanPtrByteR  /*aPacket*/)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*
    const size_t sizeLeft = aReader.SizeLeft();

    if (sizeLeft == 0)
    {
        return 0;
    }

    GpSpanPtrByteR  tryToSendDataPtr    = aReader.StoragePtr();
    ssize_t         sendSize            = send(Id(), tryToSendDataPtr.Ptr(), tryToSendDataPtr.Count(), 0);

    if (sendSize < 0)
    {
GP_WARNING_PUSH()
GP_WARNING_DISABLE(unknown-warning-option)
GP_WARNING_DISABLE(logical-op)
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
GP_WARNING_POP()
        {
            return 0;
        } else
        {
            THROW_GP(GpErrno::SGetAndClear());
        }
    }

    aReader.Offset(size_t(sendSize));

    return size_t(sendSize);*/
}

}//namespace GPlatform

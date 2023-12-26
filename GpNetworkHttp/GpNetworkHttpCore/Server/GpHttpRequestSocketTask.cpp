#include "GpHttpRequestSocketTask.hpp"

namespace GPlatform {

GpHttpRequestSocketTask::~GpHttpRequestSocketTask (void) noexcept
{
}

void    GpHttpRequestSocketTask::OnReadyToRead (GpSocket& /*aSocket*/)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*LOG_INFO
    (
        u8"[GpHttpRequestSocketTask::OnStep]: HTTP RQ:\n"_sv + iHttpRequest.V().ToString(),
        Guid()
    );

    GpHttpResponse::SP httpResponse = iHttpRequestHandler.V().ProcessRequest(iHttpRequest.V());

    if (httpResponse.V().code == GpHttpResponseCode::OK_200)
    {
        LOG_INFO
        (
            u8"[GpHttpRequestSocketTask::OnStep]: HTTP RS:\n"_sv + httpResponse.V().ToString(),
            Guid()
        );
    } else
    {
        LOG_ERROR
        (
            u8"[GpHttpRequestSocketTask::OnStep]: HTTP RS:\n"_sv + httpResponse.V().ToString(),
            Guid()
        );
    }

    iHttpRequestDoneSbr.PushEvent
    (
        MakeSP<GpHttpRequestDoneEvent>
        (
            std::move(iHttpRequest),
            std::move(httpResponse)
        )
    );

    return GpTaskDoRes::DONE;*/
}

void    GpHttpRequestSocketTask::OnReadyToWrite (GpSocket& /*aSocket*/)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpHttpRequestSocketTask::OnClosed (GpSocket& /*aSocket*/)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpHttpRequestSocketTask::OnError (GpSocket& /*aSocket*/)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

}// namespace GPlatform

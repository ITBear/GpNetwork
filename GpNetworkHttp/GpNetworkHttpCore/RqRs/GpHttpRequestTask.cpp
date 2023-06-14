#include "GpHttpRequestTask.hpp"
#include "GpHttpRequestDoneEvent.hpp"
#include "../../../../GpLog/GpLogCore/GpLog.hpp"

namespace GPlatform {

void    GpHttpRequestTask::OnStart (void)
{
    //NOP
}

GpTaskDoRes GpHttpRequestTask::OnStep (EventOptRefT /*aEvent*/)
{
    LOG_INFO
    (
        u8"[GpHttpRequestTask::OnStep]: HTTP RQ:\n"_sv + iHttpRequest.V().ToString(),
        Guid()
    );

    GpHttpResponse::SP httpResponse = iHttpRequestHandler.V().ProcessRequest(iHttpRequest.V());

    if (httpResponse.V().code == GpHttpResponseCode::OK_200)
    {
        LOG_INFO
        (
            u8"[GpHttpRequestTask::OnStep]: HTTP RS:\n"_sv + httpResponse.V().ToString(),
            Guid()
        );
    } else
    {
        LOG_ERROR
        (
            u8"[GpHttpRequestTask::OnStep]: HTTP RS:\n"_sv + httpResponse.V().ToString(),
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

    return GpTaskDoRes::DONE;
}

void    GpHttpRequestTask::OnStop (void) noexcept
{
    //NOP
}

}//namespace GPlatform

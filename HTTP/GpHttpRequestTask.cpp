#include "GpHttpRequestTask.hpp"
#include "GpHttpRequestDoneEvent.hpp"

namespace GPlatform {

GpHttpRequestTask::GpHttpRequestTask (GpHttpRequest::CSP        aHttpRequest,
                                      GpHttpRequestHandler::SP  aHttpRequestHandler,
                                      GpEventSubscriber::SP     aHttpRequestDoneSbr) noexcept:
iHttpRequest(std::move(aHttpRequest)),
iHttpRequestHandler(std::move(aHttpRequestHandler)),
iHttpRequestDoneSbr(std::move(aHttpRequestDoneSbr))
{
}

GpHttpRequestTask::~GpHttpRequestTask (void) noexcept
{
}

void    GpHttpRequestTask::OnStart (void)
{
    //NOP
}

GpTask::ResT    GpHttpRequestTask::OnStep (EventOptRefT /*aEvent*/)
{
    GpHttpResponse::SP httpResponce = iHttpRequestHandler.VC().ProcessRequest(iHttpRequest.VC());

    iHttpRequestDoneSbr->PushEvent(MakeSP<GpHttpRequestDoneEvent>(std::move(iHttpRequest),
                                                                  std::move(httpResponce)));

    return GpTask::ResT::DONE;
}

void    GpHttpRequestTask::OnStop (void) noexcept
{
    //NOP
}

}//namespace GPlatform

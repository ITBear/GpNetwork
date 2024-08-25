#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Client/GpHttpClient.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedFutureUtils.hpp>

namespace GPlatform {

GpHttpClient::GpHttpClient
(
    GpSocketFlags       aSocketFlags,
    GpIOEventPollerIdx  aIOEventPollerIdx
) noexcept:
iSocketFlags     {aSocketFlags},
iIOEventPollerIdx{aIOEventPollerIdx}
{
}

GpHttpClient::~GpHttpClient (void) noexcept
{
}

GpHttpResponse::SP  GpHttpClient::DoAndWait
(
    GpHttpRequest::SP       aRequestSP,
    const milliseconds_t    aConnectTimeout,
    const milliseconds_t    aRequestTimeout
)
{
    // TODO: create tasks pool

    // Create task
    GpHttpClientRequestTask::SP requestTaskSP = MakeSP<GpHttpClientRequestTask>
    (
        iSocketFlags,
        iIOEventPollerIdx,
        std::move(aRequestSP),
        aConnectTimeout
    );

    // Move to ready
    GpTask::DoneFutureT::SP doneFutureSP    = GpTaskScheduler::S().NewToReadyDepend(requestTaskSP);
    GpTask::DoneFutureT&    doneFuture      = doneFutureSP.V();

    // Wait for done
    GpHttpResponse::SP httpResponseSP;

    std::ignore = GpItcSharedFutureUtils::SWaitFor
    (
        doneFuture,
        [&](typename GpTaskFiber::DoneFutureT::value_type& aResult)// OnSuccessFnT
        {
            if (aResult->IsContatinType<GpHttpResponse::SP>()) [[likely]]
            {
                httpResponseSP = std::move(aResult->ValueNoCheck<GpHttpResponse::SP>());
            } else
            {
                THROW_GP
                (
                    fmt::format
                    (
                        "Unexpected result type {}",
                        aResult->TypeInfo().name()
                    )
                );
            }
        },
        [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        },
        aConnectTimeout + aRequestTimeout, // Timeout
        [&]()// On timeout
        {
            requestTaskSP.V().RequestTaskStop();

            THROW_HTTP
            (
                GpHttpExceptionCode::REQUEST_TIMEOUT_408,
                "Request timeout"
            );
        }
    );

    return httpResponseSP;
}

}// namespace GPlatform

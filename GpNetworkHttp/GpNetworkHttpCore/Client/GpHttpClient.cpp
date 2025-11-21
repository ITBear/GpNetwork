#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Client/GpHttpClient.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/ITC/GpItcFutureUtils.hpp>

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

GpHttpResponse::UP  GpHttpClient::DoAndWait
(
    GpHttpRequest::UP       aRequestUP,
    const milliseconds_t    aConnectTimeout,
    const milliseconds_t    aRequestTimeout
)
{
    // TODO: create tasks pool

    // Create task
    GpTask::SP requestTaskSP = MakeSP<GpHttpClientRequestTask>
    (
        iSocketFlags,
        iIOEventPollerIdx,
        std::move(aRequestUP),
        aConnectTimeout
    );

    // Move to ready
    GpTask::DoneFutureT::SP doneFutureSP = requestTaskSP.Vn().DoneFuture();
    SPAWN_READY_TASK(requestTaskSP);

    // Wait for done
    GpHttpResponse::UP httpResponseUP;

    std::ignore = GpItcFutureUtils::SWaitFor
    (
        doneFutureSP.V(),
        [&](typename GpTaskFiber::DoneFutureT::value_type&& aResult)// OnSuccessFnT
        {
            if (aResult->IsContatinType<GpHttpResponse::SP>()) [[likely]]
            {
                httpResponseUP = std::move(aResult->ValueNoCheck<GpHttpResponse::UP>());
            } else
            {
                THROW
                (
                    fmt::format
                    (
                        "Unexpected result type {}",
                        aResult->TypeInfo().name()
                    )
                );
            }
        },
        [](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        },
        aConnectTimeout + aRequestTimeout, // Timeout
        [&]()// On timeout
        {
            std::ignore = requestTaskSP.V().RequestStop();

            THROW
            (
                GpHttpExceptionCode::REQUEST_TIMEOUT_408,
                "Request timeout"
            );
        }
    );

    return httpResponseUP;
}

}// namespace GPlatform

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
    GpTask::DoneFutureT::C::Opts::SP doneFutureOptSP = GpTaskScheduler::S().NewToReadyDepend(requestTaskSP);

    if (doneFutureOptSP.has_value() == false)
    {
        THROW("Failed to start HTTP request task");
    }

    // Wait for done
    GpHttpResponse::SP httpResponseSP;

    std::ignore = GpItcFutureUtils::SWaitFor
    (
        doneFutureOptSP.value().V(),
        [&](typename GpTaskFiber::DoneFutureT::value_type& aResult)// OnSuccessFnT
        {
            if (aResult->IsContatinType<GpHttpResponse::SP>()) [[likely]]
            {
                httpResponseSP = std::move(aResult->ValueNoCheck<GpHttpResponse::SP>());
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

    return httpResponseSP;
}

}// namespace GPlatform

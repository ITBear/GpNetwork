#include "GpHttpClientCurlAsync.hpp"

#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpResponseCode.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Exceptions/GpHttpException.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadFixed.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkCore/GpNetworkUtilsDns.hpp>

#define CURL_STATICLIB
#include <curl/curl.h>

//TODO: see https://moz.com/devblog/high-performance-libcurl-tips
//TODO: https://curl.se/libcurl/c/ephiperfifo.html
// https://curl.se/libcurl/c/externalsocket.html

namespace GPlatform {

GP_ENUM_IMPL(GpHttpClientCurlAsyncState)

// ------------------------------------- GpHttpClientCurlAsync_S_RQ_Data_reader --------------------------------------

size_t GpHttpClientCurlAsync_S_RQ_Data_reader
(
    char*           aOutBuffer,
    const size_t    aSize,
    const size_t    aNMemb,
    GpByteReader*   aReader
)
{
    const size_t bytesLeft      = aReader->SizeLeft();
    const size_t outBufferSiz   = NumOps::SMul(aSize, aNMemb);
    const size_t batchSize      = std::min(bytesLeft, outBufferSiz);

    if (bytesLeft == 0)
    {
        return 0;
    }

    GpSpanByteR dataPartPtr = aReader->Bytes(batchSize);
    std::memcpy(aOutBuffer, dataPartPtr.Ptr(), batchSize);

    return batchSize;
}

// ------------------------------------- GpHttpClientCurlAsync_S_RS_Data_reader --------------------------------------

size_t GpHttpClientCurlAsync_S_RS_Data_reader
(
    void*                                       aPtr,
    const size_t                                aSize,
    const size_t                                aNMemb,
    std::tuple<GpByteWriter*, GpHttpHeaders*>*  aCurlAsyncRsDataReader
)
{
    GpByteWriter&   writer  = *std::get<0>(*aCurlAsyncRsDataReader);
    GpHttpHeaders&  headers = *std::get<1>(*aCurlAsyncRsDataReader);

    // Check content-length presense
    THROW_COND_GP
    (
        headers.GetValues<GpHttpHeaderType>(GpHttpHeaderType::CONTENT_LENGTH).has_value(),
        "'content-length' header is not set"
    );

    const size_t size = NumOps::SMul(aSize, aNMemb);
    writer.Bytes({reinterpret_cast<std::byte*>(aPtr), size});

    return size;
}

// ------------------------------------- GpHttpClientCurlAsync_S_RS_Headers_reader --------------------------------------

size_t GpHttpClientCurlAsync_S_RS_Headers_reader
(
    void*           aPtr,
    const size_t    aSize,
    const size_t    aNMemb,
    GpHttpHeaders*  aHeaders
)
{
    const size_t size = NumOps::SMul(aSize, aNMemb);

    if (size <= 2)
    {
        return size;
    }

    const size_t strSize = size - 2; //\r\n

    std::string_view headersStr(static_cast<const char*>(aPtr), strSize);
    std::vector<std::string_view> parts = StrOps::SSplit(headersStr, ':', 2, 0, Algo::SplitMode::COUNT_ZERO_LENGTH_PARTS);

    if (std::size(parts) >= 2)
    {
        aHeaders->GpProtoHeadersMap::Set
        (
            parts.at(0),
            std::string{parts.at(1)}
        );
    }

    return size;
}

// ------------------------------------- GpHttpClientCurlAsync --------------------------------------

GpHttpClientCurlAsync::GpHttpClientCurlAsync
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout
):
GpHttpClient
{
    aSocketFlags,
    aIOEventPollerIdx,
    aConnectTimeout
}
{
}

GpHttpClientCurlAsync::GpHttpClientCurlAsync
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout,
    std::string                 aTaskName
):
GpHttpClient
{
    aSocketFlags,
    aIOEventPollerIdx,
    aConnectTimeout,
    std::move(aTaskName)
}
{
}

GpHttpClientCurlAsync::~GpHttpClientCurlAsync (void) noexcept
{
    CurlClear();
}

GpHttpResponse::SP  GpHttpClientCurlAsync::DoRqAndWaitForRs
(
    GpHttpRequest::SP   aRequestSP,
    const ErorrMode     aErorrMode
)
{
    // Check state (expected to be READY_FOR_REQUEST). Switch to CHECK_CONNECTION
    {
        GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};

        THROW_COND_GP
        (
            iState == StateT::READY_FOR_REQUEST,
            [&]() REQUIRES(iSpinLock)
            {
                return fmt::format
                (
                    "State expected to be READY_FOR_REQUEST, but actual value {}",
                    StateT{iState}
                );
            }
        );

        // Update state to CHECK_CONNECTION
        iState = StateT::CHECK_CONNECTION;
    }

    // Check connection
    GpHttpRequest& request = aRequestSP.V();
    CheckConnection(request);

    // Do request
    return DoCurlRequestAndWait(request, aErorrMode);
}

void    GpHttpClientCurlAsync::OnReadyToRead (GpSocket& aSocket)
{
    GpHttpClient::OnReadyToRead(aSocket);
}

void    GpHttpClientCurlAsync::OnReadyToWrite (GpSocket& aSocket)
{
    GpHttpClient::OnReadyToWrite(aSocket);
}

void    GpHttpClientCurlAsync::OnClosed (GpSocket& aSocket)
{
    GpHttpClient::OnClosed(aSocket);
}

void    GpHttpClientCurlAsync::OnError (GpSocket& aSocket)
{
    GpHttpClient::OnError(aSocket);
}

void    GpHttpClientCurlAsync::ProcessOtherMessages (GpAny& aMessage)
{
    GpHttpClient::ProcessOtherMessages(aMessage);
}

GpHttpResponse::SP  GpHttpClientCurlAsync::DoCurlRequestAndWait
(
    [[maybe_unused]] GpHttpRequest& aRequest,
    [[maybe_unused]] const ErorrMode    aErorrMode
)
{
    // Get server domain name from URL
    std::string_view serverHostname = aRequest.iRequestNoBody.url.Authority().Host();

    return {};

    // 3. At this point we are connected
    // 4. Create iCurl instance
    // 5. Fill iCurl arguments
    // 6. curl_multi_add_handle
    // 7. perform curl_multi_perform with events
    // 8. read result
    // 9. check if need to keep alive


    //CurlInit();
/*
    const GpUUID        taskGuid        = GpTask::SCurrentTask().value().get().IdAsUUID();
    struct curl_slist*  curlHeadersList = nullptr;

    GpRAIIonDestruct listDestructor
    (
        [&]()
        {
            // Clean curl headers list
            if (curlHeadersList != nullptr)
            {
                curl_slist_free_all(curlHeadersList);
            }

            // Remove from iCurlMulti
            if (iCurlMulti && iCurl)
            {
                curl_multi_remove_handle(iCurlMulti, iCurl);
            }

            // Remove from io poller
            {
                const GpSocketId socketId = SocketIdNoEx();
                if (socketId != GpSocketId_Default()) [[likely]]
                {
                    GpIOEventPollerCatalog::SRemoveSubscriptionSafe(socketId, iIoEventPollerName);
                }
            }
        }
    );

    THROW_COND_GP
    (
        curl_multi_add_handle(iCurlMulti, iCurl) == CURLMcode::CURLM_OK,
        "Failed to add CURL to CURL_MULTI instance. curl_multi_add_handle() returns error"_sv
    );

    const GpHttpRequest& request        = aRequest.V();
    const GpHttpHeaders& requestHeaders = request.iRequestNoBody.headers;

    // TODO: implement GpHttpRequest and GpHttpResponse ToString method;
    const std::string urlStr = GpUrl::SToString(request.iRequestNoBody.url);
    LOG_INFO(fmt::format("CURL HTTP request to '{}'", urlStr), taskGuid);
    //LOG_INFO(request.iBody., taskGuid);
    //curl_easy_setopt(iCurl, CURLOPT_NOPROGRESS, 1L);

    if (request.iRequestNoBody.http_version == GpHttpVersion::HTTP_1_0)
    {
        curl_easy_setopt(iCurl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    } else if (request.iRequestNoBody.http_version == GpHttpVersion::HTTP_1_1)
    {
        curl_easy_setopt(iCurl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    }

    // TODO: move to config
    curl_easy_setopt(iCurl, CURLOPT_TIMEOUT, 5000L);
    curl_easy_setopt(iCurl, CURLOPT_MAXREDIRS, 2L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPINTVL, 60L);

    curl_easy_setopt(iCurl, CURLOPT_URL, std::data(urlStr));
    curl_easy_setopt(iCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(iCurl, CURLOPT_FAILONERROR, 0L);
    curl_easy_setopt(iCurl, CURLOPT_VERBOSE, 1);

    // RQ headers
    curlHeadersList = curl_slist_append(curlHeadersList, "Expect:");

    for (const auto&[name, header]: requestHeaders.Headers())
    {
        const std::string headerStr = name + ": " + StrOps::SJoin<std::string_view>(header.V().elements, ";"_sv);
        curlHeadersList = curl_slist_append(curlHeadersList, std::data(headerStr));
    }

    if (curlHeadersList != nullptr)
    {
        curl_easy_setopt(iCurl, CURLOPT_HTTPHEADER, curlHeadersList);
    }

    if (request.iRequestNoBody.request_type == GpHttpRequestType::POST)
    {
        curl_easy_setopt(iCurl, CURLOPT_POST, 1L);
    }

    // RQ data reader
    GpSpanByteR requestBody;

    if (request.iBody.IsNotNULL())
    {
        const GpHttpBodyPayload& rqBodyPayload = request.iBody.Vn();

        // TODO: implement multipart
        THROW_COND_GP
        (
            rqBodyPayload.Type() == GpHttpBodyPayloadType::FIXED_SIZE,
            "For now only FIXED_SIZE supported"
        );

        // GpHttpBodyPayloadType::FIXED_SIZE
        {
            const GpHttpBodyPayloadFixed& rqBodyPayloadFixed = static_cast<const GpHttpBodyPayloadFixed&>(rqBodyPayload);
            requestBody = rqBodyPayloadFixed.Data();
        }
    }

    GpByteReaderStorage requestBodyStorage(requestBody);
    GpByteReader        requestBodyReader(requestBodyStorage);

    if (!requestBody.Empty())
    {
        curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, GpHttpClientCurlAsync_S_RQ_Data_reader);
        curl_easy_setopt(iCurl, CURLOPT_READDATA, &requestBodyReader);
        curl_easy_setopt(iCurl, CURLOPT_POSTFIELDSIZE, NumOps::SConvert<curl_off_t>(requestBody.Count()));
    } else
    {
        curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, nullptr);
        curl_easy_setopt(iCurl, CURLOPT_READDATA, nullptr);
        curl_easy_setopt(iCurl, CURLOPT_POSTFIELDSIZE, curl_off_t(0));
    }

    // RS headers writer
    GpHttpHeaders responseHeaders;

    curl_easy_setopt(iCurl, CURLOPT_HEADERFUNCTION, GpHttpClientCurlAsync_S_RS_Headers_reader);
    curl_easy_setopt(iCurl, CURLOPT_HEADERDATA, &responseHeaders);

    // RS data reader
    GpBytesArray                    responseBody;
    responseBody.resize(4096);
    GpByteWriterStorageByteArray    responseBodyStorage(responseBody);
    GpByteWriter                    responseBodyWriter(responseBodyStorage);

    std::tuple<GpByteWriter*, GpHttpHeaders*> curlAsync_RsDataReader{&responseBodyWriter, &responseHeaders};
    curl_easy_setopt(iCurl, CURLOPT_WRITEFUNCTION, GpHttpClientCurlAsync_S_RS_Data_reader);
    curl_easy_setopt(iCurl, CURLOPT_WRITEDATA, &curlAsync_RsDataReader);

    // Add to io event poller and subscribe
    //if (!isAddedToIoEventPoller) [[unlikely]]
    {
        //isAddedToIoEventPoller = true;

        // Get event poller
        GpIOEventPoller::SP eventPoller = GpIOEventPollerCatalog::S().Get(iIoEventPollerName);

        // Add soket to event poller
        const GpTaskId taskId = GpTask::SCurrentTask().value().get().Id();
        eventPoller->AddSubscription
        (
            SocketId(),
            taskId,
            [](const GpTaskId aTaskId, [[maybe_unused]] const GpIOEventPoller::SubsriberResValT aIOEventsTypes)
            {
                GpTaskScheduler::S().MakeTaskReady(aTaskId);
            }
        );
    }

    // Do request
    int stillRunning = 1;
    //bool isAddedToIoEventPoller   = false;

    while (stillRunning > 0)
    {
        const CURLMcode res_code = curl_multi_perform(iCurlMulti, &stillRunning);

        THROW_COND_GP
        (
            res_code == CURLMcode::CURLM_OK,
            "Failed to call curl_multi_perform()"_sv
        );

        YELD_WAIT();
    }

    // Check curl res
    long httpResponseCode = 0;
    CURLcode res_code = curl_easy_getinfo(iCurl, CURLINFO_RESPONSE_CODE, &httpResponseCode);

    THROW_COND_GP
    (
        res_code == CURLE_OK,
        [&]()
        {
            return fmt::format
            (
                "curl_easy_getinfo failed: {}",
                curl_easy_strerror(res_code)
            );
        }
    );

    responseBodyWriter.OnEnd();

    GpSpanByteR responseBodyPtr(responseBody);

    LOG_INFO(responseBodyPtr.AsStringView(), taskGuid);

    if (aErorrMode == ErorrMode::THROW_ON_NOT_200)
    {
        THROW_COND_HTTP
        (
            httpResponseCode == 200,
            GpHttpResponseCode::SFromID(NumOps::SConvert<GpEnum::value_type>(httpResponseCode)),
            [&]()
            {
                return fmt::format
                (
                    "HTTP request failed (code {}): {}",
                    httpResponseCode,
                    responseBodyPtr.AsStringView()
                );
            }
        );
    }

    return MakeSP<GpHttpResponse>
    (
        GpHttpResponseNoBodyDesc
        {
            GpHttpResponseCode::SFromID(NumOps::SConvert<GpEnum::value_type>(httpResponseCode)),
            std::move(responseHeaders)
        },
        MakeSP<GpHttpBodyPayloadFixed>(std::move(responseBody))
    );
*/
}

void    GpHttpClientCurlAsync::CurlInit (void)
{
    // Curl
    {
        if (iCurl != nullptr)
        {
            return;
        }

        iCurl = curl_easy_init();

        THROW_COND_GP
        (
            iCurl != nullptr,
            "Failed to create CURL instance. curl_easy_init() returns NULL"_sv
        );

        curl_easy_setopt(iCurl, CURLOPT_PROTOCOLS,       CURLPROTO_HTTP | CURLPROTO_HTTPS);
        curl_easy_setopt(iCurl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
    }

    // CurlMulti
    {
        if (iCurlMulti != nullptr)
        {
            return;
        }

        iCurlMulti = curl_multi_init();

        THROW_COND_GP
        (
            iCurl != nullptr,
            "Failed to create CURL_MULTI instance. curl_multi_init() returns NULL"_sv
        );
    }
}

void    GpHttpClientCurlAsync::CurlClear (void) noexcept
{
    if (iCurlMulti != nullptr)
    {
        curl_multi_cleanup(iCurlMulti);
        iCurlMulti = nullptr;
    }

    if (iCurl != nullptr)
    {
        curl_easy_cleanup(iCurl);
        iCurl = nullptr;
    }
}

GpSocketId  GpHttpClientCurlAsync::SocketId (void) const
{
    if (iCurl == nullptr)
    {
        return GpSocketId_Default();
    }

    curl_socket_t   sockfd  = {};
    const auto      curlRes = curl_easy_getinfo(iCurl, CURLINFO_ACTIVESOCKET, &sockfd);

    THROW_COND_GP
    (
        curlRes == CURLE_OK,
        [curlRes]()
        {
            return fmt::format
            (
                "Failed to get active socket. Error: {}",
                curl_easy_strerror(curlRes)
            );
        }
    );

    THROW_COND_GP
    (
        sockfd != GpSocketId_Default(),
        "Failed to get active socket (default socket value returned)"
    );

    return GpSocketId{sockfd};
}

GpSocketId  GpHttpClientCurlAsync::SocketIdNoEx (void) const noexcept
{
    if (iCurl == nullptr)
    {
        return GpSocketId_Default();
    }

    curl_socket_t   sockfd  = {};
    const auto      curlRes = curl_easy_getinfo(iCurl, CURLINFO_ACTIVESOCKET, &sockfd);

    if (curlRes != CURLE_OK)
    {
        return GpSocketId_Default();
    }

    return GpSocketId{sockfd};
}

}// namespace GPlatform

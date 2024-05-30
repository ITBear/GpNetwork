/*#include "GpHttpClientCurl.hpp"

#include <GpLog/GpLogCore/GpLog.hpp>
#include <GpCore2/GpUtils/Streams/GpByteReader.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageByteArray.hpp>
#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Enums/GpHttpResponseCode.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Exceptions/GpHttpException.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayloadFixed.hpp>

#define CURL_STATICLIB
#include <curl/curl.h>

//TODO: see https://moz.com/devblog/high-performance-libcurl-tips
//TODO: https://curl.se/libcurl/c/ephiperfifo.html

namespace GPlatform {

size_t GpHttpClientCurl_S_RQ_Data_reader
(
    char*           aOutBuffer,
    size_t          aSize,
    size_t          aNMemb,
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

size_t GpHttpClientCurl_S_RS_Data_writer
(
    void*           aPtr,
    size_t          aSize,
    size_t          aNMemb,
    GpByteWriter*   aWriter
)
{
    const size_t size = NumOps::SMul(aSize, aNMemb);
    aWriter->Bytes({reinterpret_cast<std::byte*>(aPtr), size});

    return size;
}

size_t GpHttpClientCurl_S_RS_Headers_writer
(
    void*                           aPtr,
    size_t                          aSize,
    size_t                          aNMemb,
    [[maybe_unused]] GpHttpHeaders* aHeaders
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
        GpDebugging::SBreakpoint();

        // TODO: implement
        THROW_GP_NOT_IMPLEMENTED();

        //aHeaders->GpHttpProtoHeaders::Replace
        //(
        //  std::string(parts.at(0)),
        //  parts.at(1)
        //);
    }

    return size;
}

GpHttpClientCurl::GpHttpClientCurl (void) noexcept
{
}

GpHttpClientCurl::~GpHttpClientCurl (void) noexcept
{
    CurlClear();
}

GpHttpResponse::SP  GpHttpClientCurl::Do
(
    GpHttpRequest::SP   aRequest,
    const ErorrMode     aErorrMode
)
{
    const GpUUID taskGuid = GpTask::SCurrentTask().value().get().IdAsUUID();

    CurlInit();

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

    curl_easy_setopt(iCurl, CURLOPT_TIMEOUT, 5000L);
    curl_easy_setopt(iCurl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPINTVL, 60L);

    curl_easy_setopt(iCurl, CURLOPT_URL, std::data(urlStr));
    curl_easy_setopt(iCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(iCurl, CURLOPT_FAILONERROR, 0L);
    curl_easy_setopt(iCurl, CURLOPT_VERBOSE, 1);

    // RQ headers
    struct curl_slist* curlHeadersList = nullptr;

    GpRAIIonDestruct listDestructor
    (
        [&]()
        {
            if (curlHeadersList != nullptr)
            {
                curl_slist_free_all(curlHeadersList);
            }
        }
    );

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
        curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, GpHttpClientCurl_S_RQ_Data_reader);
        curl_easy_setopt(iCurl, CURLOPT_READDATA, &requestBodyReader);
        curl_easy_setopt(iCurl, CURLOPT_POSTFIELDSIZE, NumOps::SConvert<curl_off_t>(requestBody.Count()));
    } else
    {
        curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, nullptr);
        curl_easy_setopt(iCurl, CURLOPT_READDATA, nullptr);
        curl_easy_setopt(iCurl, CURLOPT_POSTFIELDSIZE, curl_off_t(0));
    }

    // RS data writer
    GpBytesArray                    responseBody;
    responseBody.resize(4096);
    GpByteWriterStorageByteArray    responseBodyStorage(responseBody);
    GpByteWriter                    responseBodyWriter(responseBodyStorage);

    curl_easy_setopt(iCurl, CURLOPT_WRITEFUNCTION, GpHttpClientCurl_S_RS_Data_writer);
    curl_easy_setopt(iCurl, CURLOPT_WRITEDATA, &responseBodyWriter);

    // RS headers writer
    GpHttpHeaders responseHeaders;
    curl_easy_setopt(iCurl, CURLOPT_HEADERFUNCTION, GpHttpClientCurl_S_RS_Headers_writer);
    curl_easy_setopt(iCurl, CURLOPT_HEADERDATA, &responseHeaders);

    // Do request
    CURLcode res_code = curl_easy_perform(iCurl);

    //Check curl res
    THROW_COND_GP
    (
        res_code == CURLE_OK,
        [&]()
        {
            return fmt::format
            (
                "curl_easy_perform failed: {}",
                curl_easy_strerror(res_code)
            );
        }
    );

    long httpResponseCode = 0;
    curl_easy_getinfo(iCurl, CURLINFO_RESPONSE_CODE, &httpResponseCode);

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
}

bool    GpHttpClientCurl::IsValid (void) const noexcept
{
    if (iCurl == nullptr)
    {
        return false;
    }

    return true;
}

void    GpHttpClientCurl::CurlInit (void)
{
    if (iCurl != nullptr)
    {
        return;
    }

    iCurl = curl_easy_init();

    THROW_COND_GP
    (
        iCurl != nullptr,
        "Failed to create CURL instance. curl_easy_init() returns NULL"
    );

    curl_easy_setopt(iCurl, CURLOPT_PROTOCOLS,       CURLPROTO_HTTP | CURLPROTO_HTTPS);
    curl_easy_setopt(iCurl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
}

void    GpHttpClientCurl::CurlClear (void) noexcept
{
    if (iCurl != nullptr)
    {
        curl_easy_cleanup(iCurl);
        iCurl = nullptr;
    }
}

}// namespace GPlatform
*/

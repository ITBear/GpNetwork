#include "GpHttpClientCurl.hpp"
#include <iostream>

#define CURL_STATICLIB
#include <curl/curl.h>

namespace GPlatform {

size_t GpHttpClientCurl_S_RQ_Data_reader (char* aOutBuffer, size_t aSize, size_t aNMemb, GpByteReader* aReader)
{
    const size_t    bytesLeft       = aReader->SizeLeft().As<size_t>();
    const size_t    outBufferSiz    = NumOps::SMul(aSize, aNMemb);
    const size_t    batchSize       = std::min(bytesLeft, outBufferSiz);

    GpRawPtrByteR       dataPartPtr = aReader->Bytes(size_byte_t::SMake(batchSize));
    GpRawPtrByteRW      outPtr(aOutBuffer, batchSize);

    outPtr.CopyFrom(dataPartPtr);

    return batchSize;
}

size_t GpHttpClientCurl_S_RS_Data_writer (void* aPtr, size_t aSize, size_t aNMemb, GpByteWriter* aWriter)
{
    const size_t size = NumOps::SMul(aSize, aNMemb);
    aWriter->Bytes({reinterpret_cast<std::byte*>(aPtr), size});
    return size;
}

size_t GpHttpClientCurl_S_RS_Headers_writer (void* aPtr, size_t aSize, size_t aNMemb, GpHttpHeaders* aHeaders)
{
    const size_t size = NumOps::SMul(aSize, aNMemb);

    GpRawPtrCharR headersStr(static_cast<const char*>(aPtr), size);
    GpVector<GpRawPtrCharR> parts = GpStringOps::SSplit(headersStr, ':', 2_cnt, 1_cnt, Algo::SplitMode::COUNT_ZERO_LENGTH_PARTS);

    aHeaders->Replace(std::string(parts.at(0).AsStringView()),
                      std::string(parts.at(1).AsStringView()));

    return size;
}

GpHttpClientCurl::GpHttpClientCurl (void) noexcept
{
}

GpHttpClientCurl::~GpHttpClientCurl (void) noexcept
{
    CurlClear();
}

GpHttpResponse::SP  GpHttpClientCurl::Do (GpHttpRequest::SP aRequest)
{
    CurlInit();

    const GpHttpRequest&            request         = aRequest.VC();
    const GpHttpHeaders&            requestHeaders  = request.headers;
    const GpBytesArray&             requestBody     = request.body;
    const size_t                    requestBodySize = requestBody.size();
    GpByteReaderStorage             requestBodyStorage(requestBody);
    GpByteReader                    requestBodyReader(requestBodyStorage);
    GpBytesArray                    responseBody;
    GpByteWriterStorageByteArray    responseBodyStorage(responseBody);
    GpByteWriter                    responseBodyWriter(responseBodyStorage);

    //curl_easy_setopt(iCurl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(iCurl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(iCurl, CURLOPT_TCP_KEEPINTVL, 60L);

    curl_easy_setopt(iCurl, CURLOPT_URL, request.url.data());
    curl_easy_setopt(iCurl, CURLOPT_SSL_VERIFYPEER, 0);
    //curl_easy_setopt(iCurl, CURLOPT_VERBOSE, 1);

    //RQ headers
    struct curl_slist* curlHeadersList = nullptr;

    GpRAIIonDestruct listDestructor([&]()
    {
        if (curlHeadersList != nullptr)
        {
            curl_slist_free_all(curlHeadersList);
        }
    });

    for (const auto&[name, header]: requestHeaders.headers)
    {
        std::string headerStr = name + ": "_sv + GpStringOps::SJoin<std::string_view>(header.VC().elements, ";"_sv);
        curlHeadersList = curl_slist_append(curlHeadersList, headerStr.data());
    }

    if (curlHeadersList != nullptr)
    {
        curl_easy_setopt(iCurl, CURLOPT_HTTPHEADER, curlHeadersList);
    }

    //RQ data reader
    if (requestBodySize > 0)
    {
        curl_easy_setopt(iCurl, CURLOPT_POST, 1L);
        curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, GpHttpClientCurl_S_RQ_Data_reader);
        curl_easy_setopt(iCurl, CURLOPT_READDATA, &requestBodyReader);
        curl_easy_setopt(iCurl, CURLOPT_POSTFIELDSIZE, NumOps::SConvert<curl_off_t>(requestBodySize));
    } else
    {
        curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, nullptr);
        curl_easy_setopt(iCurl, CURLOPT_READDATA, nullptr);
        curl_easy_setopt(iCurl, CURLOPT_POSTFIELDSIZE, curl_off_t(0));
    }

    //RS data writer
    curl_easy_setopt(iCurl, CURLOPT_WRITEFUNCTION, GpHttpClientCurl_S_RS_Data_writer);
    curl_easy_setopt(iCurl, CURLOPT_WRITEDATA, &responseBodyWriter);

    //RS headers writer
    GpHttpHeaders responseHeaders;
    curl_easy_setopt(iCurl, CURLOPT_HEADERFUNCTION, GpHttpClientCurl_S_RS_Headers_writer);
    curl_easy_setopt(iCurl, CURLOPT_HEADERDATA, &responseHeaders);

    //Do request
    std::cout << "--------------------------------- BEGIN -------------------------------" << std::endl;
    CURLcode res_code = curl_easy_perform(iCurl);
    std::cout << "---------------------------------- END --------------------------------" << std::endl;

    //Check curl res
    THROW_GPE_COND_CHECK_M(res_code == CURLE_OK,
                           "curl_easy_perform failed: "_sv + curl_easy_strerror(res_code));

    //Check HTTP code
    {
        long httpResponseCode = 0;
        curl_easy_getinfo(iCurl, CURLINFO_RESPONSE_CODE, &httpResponseCode);

        GpRawPtrByteR responseBodyPtr(responseBody);

        THROW_HTTP_COND_CHECK_M(httpResponseCode == 200,
                                GpHttpResponseCodeUtils::SFromId(httpResponseCode),
                                "HTTP request failed (code "_sv + httpResponseCode + "): "_sv + responseBodyPtr.AsStringView());
    }

    return MakeSP<GpHttpResponse>(GpHttpResponseCode::OK_200,
                                  std::move(responseHeaders),
                                  std::move(responseBody));
}

void    GpHttpClientCurl::CurlInit (void)
{
    if (iCurl != nullptr)
    {
        return;
    }

    iCurl = curl_easy_init();
    THROW_GPE_COND_CHECK_M(iCurl != nullptr, "Failed to create curl instance"_sv);
}

void    GpHttpClientCurl::CurlClear (void) noexcept
{
    if (iCurl != nullptr)
    {
        curl_easy_cleanup(iCurl);
        iCurl = nullptr;
    }
}

}//namespace GPlatform

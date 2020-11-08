#include "GpHttpClient.hpp"
#include <iostream>

#define CURL_STATICLIB
#include <curl/curl.h>

namespace GPlatform {

size_t GpHTTPClient_S_RQ_Data_reader (char* aOutBuffer, size_t aSize, size_t aNMemb, GpByteReader* aReader)
{
    const size_t    bytesLeft       = aReader->SizeLeft().As<size_t>();
    const size_t    outBufferSiz    = NumOps::SMul(aSize, aNMemb);
    const size_t    batchSize       = std::min(bytesLeft, outBufferSiz);

    GpRawPtrByteR       dataPartPtr = aReader->Bytes(size_byte_t::SMake(batchSize));
    GpRawPtrByteRW      outPtr(aOutBuffer, batchSize);

    outPtr.CopyFrom(dataPartPtr);

    return batchSize;
}

size_t GpHTTPClient_S_RS_Data_writer (void* aPtr, size_t aSize, size_t aNMemb, GpByteWriter* aWriter)
{
    const size_t size = NumOps::SMul(aSize, aNMemb);
    aWriter->Bytes({reinterpret_cast<std::byte*>(aPtr), size});
    return size;
}

GpHttpClient::GpHttpClient (void) noexcept
{
}

GpHttpClient::~GpHttpClient (void) noexcept
{
    CurlClear();
}

GpHttpResponse::SP  GpHttpClient::Do (GpHttpRequest::SP aRequest)
{
    CurlInit();

    const GpHttpRequest&            request         = aRequest.VC();
    const GpBytesArray&             requestBody     = request.Body();
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

    curl_easy_setopt(iCurl, CURLOPT_URL, request.Url().data());
    curl_easy_setopt(iCurl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(iCurl, CURLOPT_VERBOSE, 1);

    //RQ data reader
    if (requestBodySize > 0)
    {
        curl_easy_setopt(iCurl, CURLOPT_POST, 1L);
        curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, GpHTTPClient_S_RQ_Data_reader);
        curl_easy_setopt(iCurl, CURLOPT_READDATA, &requestBodyReader);
        curl_easy_setopt(iCurl, CURLOPT_POSTFIELDSIZE, NumOps::SConvert<curl_off_t>(requestBodySize));
    } else
    {
        curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, nullptr);
        curl_easy_setopt(iCurl, CURLOPT_READDATA, nullptr);
        curl_easy_setopt(iCurl, CURLOPT_POSTFIELDSIZE, curl_off_t(0));
    }

    //RS data writer
    curl_easy_setopt(iCurl, CURLOPT_WRITEFUNCTION, GpHTTPClient_S_RS_Data_writer);
    curl_easy_setopt(iCurl, CURLOPT_WRITEDATA, &responseBodyWriter);

    //Do request
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! BEGIN" << std::endl;
    CURLcode res_code = curl_easy_perform(iCurl);
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! END" << std::endl;

    //Check curl res
    THROW_GPE_COND_CHECK_M(res_code == CURLE_OK,
                           "curl_easy_perform failed: "_sv + curl_easy_strerror(res_code));

    //Check HTTP code
    {
        long httpResponseCode = 0;
        curl_easy_getinfo(iCurl, CURLINFO_RESPONSE_CODE, &httpResponseCode);

        GpRawPtrByteR responseBodyPtr(responseBody);

        THROW_GPE_COND_CHECK_M(httpResponseCode == 200,
                               "HTTP request failed (code "_sv + httpResponseCode + "): "_sv + responseBodyPtr.AsStringView());
    }

    //TODO add value reader from curl
    return MakeSP<GpHttpResponse>(GpHttpVersion::HTTP_1_1,
                                  GpHttpResponseCode::OK_200,
                                  GpHttpContentType::APPLICATION_JSON,
                                  GpHttpCharset::UTF_8,
                                  GpHttpConnectionFlag::KEEP_ALIVE,
                                  GpHttpCacheControl::NO_STORE,
                                  std::move(responseBody));
}

void    GpHttpClient::CurlInit (void)
{
    if (iCurl != nullptr)
    {
        return;
    }

    iCurl = curl_easy_init();
    THROW_GPE_COND_CHECK_M(iCurl != nullptr, "Failed to create curl instance"_sv);
}

void    GpHttpClient::CurlClear (void) noexcept
{
    if (iCurl != nullptr)
    {
        curl_easy_cleanup(iCurl);
        iCurl = nullptr;
    }
}

}//namespace GPlatform

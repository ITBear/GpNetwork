#include "GpSmtpClientCurl.hpp"
#include <iostream>

#define CURL_STATICLIB
#include <curl/curl.h>

namespace GPlatform {

size_t GpSmtpClientCurl_S_RQ_Data_reader
(
    char*           aOutBuffer,
    size_t          aSize,
    size_t          aNMemb,
    GpByteReader*   aReader
)
{
    const size_t    bytesLeft       = aReader->SizeLeft().As<size_t>();
    const size_t    outBufferSiz    = NumOps::SMul(aSize, aNMemb);
    const size_t    batchSize       = std::min(bytesLeft, outBufferSiz);

    if (bytesLeft == 0)
    {
        return 0;
    }

    GpRawPtrByteR   dataPartPtr = aReader->Bytes(size_byte_t::SMake(batchSize));
    GpRawPtrByteRW  outPtr(aOutBuffer, batchSize);

    outPtr.CopyFrom(dataPartPtr);

    return batchSize;
}

GpSmtpClientCurl::GpSmtpClientCurl (void) noexcept
{
}

GpSmtpClientCurl::~GpSmtpClientCurl (void) noexcept
{
    CurlClear();
}

std::string GpSmtpClientCurl::Send (const GpEmail& aEmail)
{
    //https://curl.se/libcurl/c/smtp-ssl.html
    //https://curl.se/libcurl/c/smtp-tls.html

    CurlInit();

    GpBytesArray body;
    body.reserve(4096);
    GpByteWriterStorageByteArray    bodyWriterStorage(body);
    GpByteWriter                    bodyWriter(bodyWriterStorage);

    const std::string messageId = MakeBody(aEmail, bodyWriter);

    //std::cout << "[GpSmtpClientCurl::Send]: Email body: '" << GpRawPtrByteR(body).AsStringView() << std::endl;
    //std::cout << "[GpSmtpClientCurl::Send]: Email ID: '" << messageId << "'" << std::endl;

    GpByteReaderStorage bodyStorage(body);
    GpByteReader        bodyReader(bodyStorage);

    //curl_easy_setopt(iCurl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(iCurl, CURLOPT_USERNAME, Login().data());
    curl_easy_setopt(iCurl, CURLOPT_PASSWORD, Password().data());
    curl_easy_setopt(iCurl, CURLOPT_URL, Smtp().data());
    curl_easy_setopt(iCurl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(iCurl, CURLOPT_READFUNCTION, GpSmtpClientCurl_S_RQ_Data_reader);
    curl_easy_setopt(iCurl, CURLOPT_READDATA, &bodyReader);
    curl_easy_setopt(iCurl, CURLOPT_UPLOAD, 1L);
    //curl_easy_setopt(iCurl, CURLOPT_VERBOSE, 1L);

    //From
    std::string_view fromAddr = aEmail.from.addr;
    THROW_GPE_COND
    (
        fromAddr.length() > 0,
        "From is empty"_sv
    );

    curl_easy_setopt(iCurl, CURLOPT_MAIL_FROM, fromAddr.data());

    //Recipients
    struct curl_slist* curlRecipientsList = nullptr;

    GpRAIIonDestruct listDestructor([&]()
    {
        if (curlRecipientsList != nullptr)
        {
            curl_slist_free_all(curlRecipientsList);
        }
    });

    FillAddrs(&curlRecipientsList, aEmail.to);
    FillAddrs(&curlRecipientsList, aEmail.cc);

    if (curlRecipientsList != nullptr)
    {
        curl_easy_setopt(iCurl, CURLOPT_MAIL_RCPT, curlRecipientsList);
    }

    //Send
    CURLcode curlResCode = curl_easy_perform(iCurl);

    //Check
    THROW_GPE_COND
    (
        curlResCode == CURLE_OK,
        [&](){return "curl_easy_perform failed: "_sv + curl_easy_strerror(curlResCode);}
    );

    return messageId;
}

bool    GpSmtpClientCurl::IsValid (void) const noexcept
{
    if (iCurl == nullptr)
    {
        return false;
    }

    return true;
}

void    GpSmtpClientCurl::CurlInit (void)
{
    if (iCurl != nullptr)
    {
        return;
    }

    iCurl = curl_easy_init();

    THROW_GPE_COND
    (
        iCurl != nullptr,
        "Failed to create curl instance"_sv
    );
}

void    GpSmtpClientCurl::CurlClear (void) noexcept
{
    if (iCurl != nullptr)
    {
        curl_easy_cleanup(iCurl);
        iCurl = nullptr;
    }
}

void    GpSmtpClientCurl::FillAddrs
(
    void*                           aCurlList,
    const GpEmailAddr::C::Vec::SP&  aAddrList
) const
{
    struct curl_slist** curlList = static_cast<struct curl_slist**>(aCurlList);

    for (const auto& element: aAddrList)
    {
        std::string_view addr = element.VC().addr;

        THROW_GPE_COND
        (
            addr.length() > 0,
            "Addr is empty"_sv
        );

        *curlList = curl_slist_append(*curlList, addr.data());
    }
}

}//namespace GPlatform

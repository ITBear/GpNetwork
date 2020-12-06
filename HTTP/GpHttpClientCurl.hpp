#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"

typedef void CURL;

namespace GPlatform {

class GPNETWORK_API GpHttpClientCurl
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpClientCurl)
    CLASS_DECLARE_DEFAULTS(GpHttpClientCurl)

    enum class ErorrMode
    {
        THROW_ON_NOT_200,
        RETURN_RS
    };

public:
                                GpHttpClientCurl    (void) noexcept;
                                ~GpHttpClientCurl   (void) noexcept;

    GpHttpResponse::SP          Do                  (GpHttpRequest::SP  aRequest,
                                                     const ErorrMode    aErorrMode);

private:
    void                        CurlInit            (void);
    void                        CurlClear           (void) noexcept;

private:
    CURL*                       iCurl = nullptr;
};

}//namespace GPlatform

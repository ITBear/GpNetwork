#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"

typedef void CURL;

namespace GPlatform {

class GPNETWORK_API GpHttpClient
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpClient)
    CLASS_DECLARE_DEFAULTS(GpHttpClient)

public:
                                GpHttpClient    (void) noexcept;
                                ~GpHttpClient   (void) noexcept;

    GpHttpResponse::SP          Do              (GpHttpRequest::SP aRequest);

private:
    void                        CurlInit        (void);
    void                        CurlClear       (void) noexcept;

private:
    CURL*                       iCurl = nullptr;
};

}//namespace GPlatform

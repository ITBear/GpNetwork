#pragma once

#include "GpHttpClient.hpp"

typedef void CURL;

namespace GPlatform {

class GPNETWORK_API GpHttpClientCurl final: public GpHttpClient
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpClientCurl)
    CLASS_DECLARE_DEFAULTS(GpHttpClientCurl)

public:
                                GpHttpClientCurl    (void) noexcept;
    virtual                     ~GpHttpClientCurl   (void) noexcept override final;

    virtual GpHttpResponse::SP  Do                  (GpHttpRequest::SP  aRequest,
                                                     const ErorrMode    aErorrMode) override final;

    virtual bool                IsValid             (void) const noexcept override final;

private:
    void                        CurlInit            (void);
    void                        CurlClear           (void) noexcept;

private:
    CURL*                       iCurl = nullptr;
};

}//namespace GPlatform

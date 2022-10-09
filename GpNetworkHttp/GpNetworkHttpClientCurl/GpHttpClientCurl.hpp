#pragma once

#include "GpNetworkHttpClientCurl_global.hpp"

typedef void CURL;

namespace GPlatform {

class GpHttpClientCurl final: public GpHttpClient
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpClientCurl)
    CLASS_DD(GpHttpClientCurl)

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

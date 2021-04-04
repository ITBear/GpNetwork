#pragma once

#include "GpHttpClient.hpp"

typedef void CURL;
typedef void CURLM;

namespace GPlatform {

/*class GPNETWORK_API GpHttpClientCurlAsync final: public GpHttpClient
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpClientCurlAsync)
    CLASS_DECLARE_DEFAULTS(GpHttpClientCurlAsync)

public:
                                GpHttpClientCurlAsync   (void) noexcept;
    virtual                     ~GpHttpClientCurlAsync  (void) noexcept override final;

    virtual GpHttpResponse::SP  Do                      (GpHttpRequest::SP  aRequest,
                                                         const ErorrMode    aErorrMode) override final;

    virtual bool                IsValid                 (void) const noexcept override final;

private:
    void                        CurlInit                (void);
    void                        CurlClear               (void) noexcept;

private:
    CURL*                       iCurl = nullptr;
};*/

}//namespace GPlatform

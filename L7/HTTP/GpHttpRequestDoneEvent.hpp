#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequestDoneEvent final: public GpEvent
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpRequestDoneEvent)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestDoneEvent)
    TYPE_STRUCT_DECLARE("384c1d20-e12b-460a-b2ec-990b54158090"_sv)

public:
                                GpHttpRequestDoneEvent  (void) noexcept;
                                GpHttpRequestDoneEvent  (GpHttpRequest::CSP aRequest,
                                                         GpHttpResponse::SP aResponse) noexcept;
    virtual                     ~GpHttpRequestDoneEvent (void) noexcept override final;

    GpHttpRequest::CSP          Request                 (void) const {return iRequest;}
    GpHttpResponse::SP          Response                (void) const {return iResponse;}

private:
    GpHttpRequest::CSP          iRequest;
    GpHttpResponse::SP          iResponse;
};

}//namespace GPlatform

#pragma once

#include "GpHttpRequest.hpp"
#include "GpHttpResponse.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestDoneEvent final: public GpEvent
{
public:
    CLASS_DD(GpHttpRequestDoneEvent)

public:
                                GpHttpRequestDoneEvent  (void) noexcept = default;
    inline                      GpHttpRequestDoneEvent  (const GpHttpRequestDoneEvent& aEvent);
    inline                      GpHttpRequestDoneEvent  (GpHttpRequestDoneEvent&& aEvent) noexcept;
    inline                      GpHttpRequestDoneEvent  (GpHttpRequest::SP  aRequest,
                                                         GpHttpResponse::SP aResponse) noexcept;
    virtual                     ~GpHttpRequestDoneEvent (void) noexcept override final;

    static const GpUUID&        SType                   (void) noexcept {return sTypeId;}
    virtual const GpUUID&       Type                    (void) const noexcept override final;

    GpHttpRequest::SP           Request                 (void) const {return iRequest;}
    GpHttpResponse::SP          Response                (void) const {return iResponse;}

private:
    GpHttpRequest::SP           iRequest;
    GpHttpResponse::SP          iResponse;

    static const GpUUID         sTypeId;
};

GpHttpRequestDoneEvent::GpHttpRequestDoneEvent (const GpHttpRequestDoneEvent& aEvent):
GpEvent(aEvent),
iRequest (GpReflectUtils::SCopyValue(aEvent.iRequest)),
iResponse(GpReflectUtils::SCopyValue(aEvent.iResponse))
{
}

GpHttpRequestDoneEvent::GpHttpRequestDoneEvent (GpHttpRequestDoneEvent&& aEvent) noexcept:
GpEvent(std::move(aEvent)),
iRequest (std::move(aEvent.iRequest)),
iResponse(std::move(aEvent.iResponse))
{
}

GpHttpRequestDoneEvent::GpHttpRequestDoneEvent
(
    GpHttpRequest::SP   aRequest,
    GpHttpResponse::SP  aResponse
) noexcept:
iRequest (std::move(aRequest)),
iResponse(std::move(aResponse))
{
}

}//namespace GPlatform

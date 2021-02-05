#include "GpHttpRequestDoneEvent.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpHttpRequestDoneEvent, GP_MODULE_UUID)

GpHttpRequestDoneEvent::GpHttpRequestDoneEvent (void) noexcept
{
}

GpHttpRequestDoneEvent::GpHttpRequestDoneEvent (GpHttpRequest::CSP  aRequest,
                                                GpHttpResponse::SP  aResponse) noexcept:
iRequest(std::move(aRequest)),
iResponse(std::move(aResponse))
{
}

GpHttpRequestDoneEvent::~GpHttpRequestDoneEvent (void) noexcept
{
}

void    GpHttpRequestDoneEvent::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& /*aPropsOut*/)
{
    //PROP(iRequest);
    //PROP(iResponse);
}

}//namespace GPlatform

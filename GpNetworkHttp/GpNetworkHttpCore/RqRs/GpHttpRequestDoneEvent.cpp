#include "GpHttpRequestDoneEvent.hpp"

namespace GPlatform {

const GpUUID    GpHttpRequestDoneEvent::sTypeId = "09b6e79d-670f-4887-defb-ec34ea3bada3"_uuid;

GpHttpRequestDoneEvent::~GpHttpRequestDoneEvent (void) noexcept
{
}

const GpUUID&   GpHttpRequestDoneEvent::Type (void) const noexcept
{
    return GpHttpRequestDoneEvent::SType();
}

}//namespace GPlatform

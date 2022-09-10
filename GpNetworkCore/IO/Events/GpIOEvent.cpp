#include "GpIOEvent.hpp"

namespace GPlatform {

const GpUUID    GpIOEvent::sTypeId = "5dd0e5fc-200f-4190-9447-968b643cf661"_uuid;

GpIOEvent::~GpIOEvent (void) noexcept
{
}

const GpUUID&   GpIOEvent::Type (void) const noexcept
{
    return GpIOEvent::SType();
}

}//namespace GPlatform

#pragma once

#include "GpIOEventType.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEvent final: public GpEvent
{
public:
    CLASS_DD(GpIOEvent)

public:
                            GpIOEvent       (void) noexcept = default;
                            GpIOEvent       (const GpIOEvent& aEvent) noexcept: iIOEvents(aEvent.iIOEvents) {}
                            GpIOEvent       (GpIOEvent&& aEvent) noexcept: iIOEvents(std::move(aEvent.iIOEvents)) {}
                            GpIOEvent       (const GpIOEventsTypes& aIOEvents) noexcept: iIOEvents(aIOEvents) {}
    virtual                 ~GpIOEvent      (void) noexcept override final;

    static const GpUUID&    SType           (void) noexcept {return sTypeId;}
    virtual const GpUUID&   Type            (void) const noexcept override final;

    const GpIOEventsTypes&  Events          (void) const noexcept {return iIOEvents;}

private:
    GpIOEventsTypes         iIOEvents;

    static const GpUUID     sTypeId;
};

}//namespace GPlatform

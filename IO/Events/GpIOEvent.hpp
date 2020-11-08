#pragma once

#include "GpIOEventType.hpp"

namespace GPlatform {

class GpIOEvent final: public GpEvent
{
public:
    CLASS_REMOVE_CTRS(GpIOEvent)
    CLASS_DECLARE_DEFAULTS(GpIOEvent)

public:
                            GpIOEvent       (const GpIOEventsTypes& aIOEvents) noexcept: iIOEvents(aIOEvents) {}
    virtual                 ~GpIOEvent      (void) noexcept override final {}

    const GpIOEventsTypes&  Events          (void) const noexcept {return iIOEvents;}

private:
    GpIOEventsTypes         iIOEvents;
};

}//namespace GPlatform

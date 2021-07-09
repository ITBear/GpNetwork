#pragma once

#include "GpIOEventType.hpp"

namespace GPlatform {

class GpIOEvent final: public GpEvent
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEvent)
    CLASS_DECLARE_DEFAULTS(GpIOEvent)
    TYPE_STRUCT_DECLARE("5b86e9a5-4e36-4caa-b0da-a1348ad53440"_sv)

public:
                            GpIOEvent       (void) noexcept {}
                            GpIOEvent       (const GpIOEventsTypes& aIOEvents) noexcept: iIOEvents(aIOEvents) {}
    virtual                 ~GpIOEvent      (void) noexcept override final {}

    const GpIOEventsTypes&  Events          (void) const noexcept {return iIOEvents;}

private:
    GpIOEventsTypes         iIOEvents;
};

}//namespace GPlatform

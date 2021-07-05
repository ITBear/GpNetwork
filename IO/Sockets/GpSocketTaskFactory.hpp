#pragma once

#include "GpSocketTask.hpp"

namespace GPlatform {

class GPNETWORK_API GpSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpSocketTaskFactory)
    CLASS_DECLARE_DEFAULTS(GpSocketTaskFactory)

public:
                                GpSocketTaskFactory     (void) noexcept {}
    virtual                     ~GpSocketTaskFactory    (void) noexcept {}

    virtual GpSocketTask::SP    NewInstance             (std::string_view       aName,
                                                         GpIOEventPoller::WP    aIOPoller,
                                                         GpSocket::SP           aSocket) const = 0;
};

}//namespace GPlatform

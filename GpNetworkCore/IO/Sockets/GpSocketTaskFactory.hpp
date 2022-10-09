#pragma once

#include "GpSocketTask.hpp"

namespace GPlatform {

class GpSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSocketTaskFactory)
    CLASS_DD(GpSocketTaskFactory)

public:
                                GpSocketTaskFactory     (void) noexcept = default;
    virtual                     ~GpSocketTaskFactory    (void) noexcept = default;

    virtual GpSocketTask::SP    NewInstance             (std::string            aName,
                                                         GpIOEventPoller::SP    aIOPoller,
                                                         GpSocket::SP           aSocket) const = 0;
};

}//namespace GPlatform

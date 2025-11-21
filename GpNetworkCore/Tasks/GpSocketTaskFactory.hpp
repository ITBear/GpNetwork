#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSocketTask.hpp>

namespace GPlatform {

class GpSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSocketTaskFactory)
    CLASS_DD(GpSocketTaskFactory)

protected:
                                GpSocketTaskFactory     (void) noexcept = default;

public:
    virtual                     ~GpSocketTaskFactory    (void) noexcept = default;

    virtual GpSocketTask::SP    NewInstance             (GpSocket::UP aSocket) const = 0;
};

}// namespace GPlatform

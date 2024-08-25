#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpTcpServerTask.hpp>

namespace GPlatform {

class GpTcpServerTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTcpServerTaskFactory)
    CLASS_DD(GpTcpServerTaskFactory)

protected:
                                    GpTcpServerTaskFactory  (void) noexcept = default;

public:
    virtual                         ~GpTcpServerTaskFactory (void) noexcept = default;

    virtual GpTcpServerTask::SP     NewInstance             (GpSocketTCP::SP    aSocketTCP,
                                                             GpIOEventPollerIdx aIOEventPollerIdx) const = 0;
};

}// namespace GPlatform

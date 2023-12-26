#pragma once

#include "GpSingleSocketTask.hpp"

namespace GPlatform {

class GpSingleSocketTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSingleSocketTaskFactory)
    CLASS_DD(GpSingleSocketTaskFactory)

protected:
                                    GpSingleSocketTaskFactory   (void) noexcept = default;

public:
    virtual                         ~GpSingleSocketTaskFactory  (void) noexcept = default;

    virtual GpSingleSocketTask::SP  NewInstance                 (GpSocket::SP aSocket) const = 0;
};

}//namespace GPlatform

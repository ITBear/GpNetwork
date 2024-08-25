#pragma once

#include "GpSocket.hpp"
#include <GpCore2/GpTasks/GpTaskEnums.hpp>

namespace GPlatform {

class GpSocketFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSocketFactory)
    CLASS_DD(GpSocketFactory)

protected:
                            GpSocketFactory     (void) noexcept = default;

public:
    virtual                 ~GpSocketFactory    (void) noexcept = default;

    virtual GpSocket::SP    NewInstance         (void) const = 0;
};

}// namespace GPlatform

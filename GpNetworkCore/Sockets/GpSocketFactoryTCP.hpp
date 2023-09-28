#pragma once

#include "GpSocketFactory.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketFactoryTCP final: public GpSocketFactory
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSocketFactoryTCP)
    CLASS_DD(GpSocketFactoryTCP)

public:
    inline                      GpSocketFactoryTCP  (const GpSocketFlags&           aFlags,
                                                     const GpSocket::CloseModeT     aCloseMode) noexcept;
    virtual                     ~GpSocketFactoryTCP (void) noexcept override final = default;

    virtual GpSocket::SP        NewInstance         (void) const override final;

private:
    const GpSocketFlags         iFlags;
    const GpSocket::CloseModeT  iCloseMode;
};

GpSocketFactoryTCP::GpSocketFactoryTCP
(
    const GpSocketFlags&        aFlags,
    const GpSocket::CloseModeT  aCloseMode
) noexcept:
iFlags    (aFlags),
iCloseMode(aCloseMode)
{
}

}//namespace GPlatform

#include "GpIOEventPollerSelectFactory.hpp"

#include "GpIOEventPollerSelect.hpp"

namespace GPlatform {

GpIOEventPollerSelectFactory::~GpIOEventPollerSelectFactory (void) noexcept
{
}

GpIOEventPoller::SP GpIOEventPollerSelectFactory::NewInstance (std::string aName) const
{
    GpIOEventPollerSelect::SP SelectSP = MakeSP<GpIOEventPollerSelect>(std::move(aName));

    SelectSP.V().Configure(iMaxStepTime);

    return SelectSP;
}

}// namespace GPlatform

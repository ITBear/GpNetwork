#include <GpNetwork/GpNetworkCore/Tasks/GpSocketTask.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp>

namespace GPlatform {

GpSocketTask::GpSocketTask (void) noexcept:
GpTaskFiber{}
{
}

GpSocketTask::GpSocketTask (std::string aTaskName) noexcept:
GpTaskFiber{std::move(aTaskName)}
{
}

GpSocketTask::~GpSocketTask (void) noexcept
{
}

u_int_64    GpSocketTask::TypeUID(void) const noexcept
{
    return STypeUID();
}

}// namespace GPlatform

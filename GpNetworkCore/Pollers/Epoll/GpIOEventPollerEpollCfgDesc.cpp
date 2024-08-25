#include <GpNetwork/GpNetworkCore/Pollers/Epoll/GpIOEventPollerEpollCfgDesc.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

#if defined(GP_OS_LINUX)

REFLECT_IMPLEMENT(GpIOEventPollerEpollCfgDesc, GP_MODULE_UUID)

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (const GpIOEventPollerEpollCfgDesc& aDesc) noexcept:
GpIOEventPollerCfgDesc{aDesc},
max_step_time {GpReflectUtils::SCopyValue(aDesc.max_step_time)},
max_events_cnt{GpReflectUtils::SCopyValue(aDesc.max_events_cnt)}
{
}

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc (GpIOEventPollerEpollCfgDesc&& aDesc) noexcept:
GpIOEventPollerCfgDesc{std::move(aDesc)},
max_step_time {std::move(aDesc.max_step_time)},
max_events_cnt{std::move(aDesc.max_events_cnt)}
{
}

GpIOEventPollerEpollCfgDesc::GpIOEventPollerEpollCfgDesc
(
    const milliseconds_t    aMaxStepTime,
    const size_t            aMaxEventsCnt
) noexcept:
max_step_time {std::move(aMaxStepTime)},
max_events_cnt{std::move(aMaxEventsCnt)}
{
}

GpIOEventPollerEpollCfgDesc::~GpIOEventPollerEpollCfgDesc (void) noexcept
{
}

void    GpIOEventPollerEpollCfgDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(max_step_time);
    PROP(max_events_cnt);
}

#endif// #if defined(GP_OS_LINUX)

}// namespace GPlatform

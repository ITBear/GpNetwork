#include "GpProtoHeaderValue.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpProtoHeaderValue, GP_MODULE_UUID)

GpProtoHeaderValue::~GpProtoHeaderValue (void) noexcept
{
}

void    GpProtoHeaderValue::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(elements);
}

}// namespace GPlatform

#include "GpProtoHeaderValue.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpProtoHeaderValue, GP_MODULE_UUID)

GpProtoHeaderValue::~GpProtoHeaderValue (void) noexcept
{
}

void    GpProtoHeaderValue::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(elements);
}

}//namespace GPlatform

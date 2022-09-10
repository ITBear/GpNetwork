#include "GpProtoHeaderValue.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpProtoHeaderValue, GP_MODULE_UUID)

GpProtoHeaderValue::GpProtoHeaderValue (void) noexcept
{
}

GpProtoHeaderValue::GpProtoHeaderValue (std::string_view aElement)
{
    elements.emplace_back(std::move(aElement));
}

GpProtoHeaderValue::GpProtoHeaderValue (std::string&& aElement)
{
    elements.emplace_back(std::move(aElement));
}

GpProtoHeaderValue::GpProtoHeaderValue (const GpProtoHeaderValue& aValue):
GpReflectObject(aValue),
elements(aValue.elements)
{
}

GpProtoHeaderValue::GpProtoHeaderValue (GpProtoHeaderValue&& aValue) noexcept:
GpReflectObject(std::move(aValue)),
elements(std::move(aValue.elements))
{
}

GpProtoHeaderValue::~GpProtoHeaderValue (void) noexcept
{
}

void    GpProtoHeaderValue::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(elements);
}

}//namespace GPlatform

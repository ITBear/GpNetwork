#include "GpProtoHeaderValue.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpProtoHeaderValue, GP_MODULE_UUID)

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
elements(aValue.elements)
{
}

GpProtoHeaderValue::GpProtoHeaderValue (GpProtoHeaderValue&& aValue) noexcept:
elements(std::move(aValue.elements))
{
}

GpProtoHeaderValue::~GpProtoHeaderValue (void) noexcept
{
}

void    GpProtoHeaderValue::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(elements);
}

}//namespace GPlatform

#include "GpHttpHeaderValue.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpHttpHeaderValue, GP_MODULE_UUID)

GpHttpHeaderValue::GpHttpHeaderValue (void) noexcept
{
}

GpHttpHeaderValue::GpHttpHeaderValue (std::string&& aElement)
{
    elements.emplace_back(std::move(aElement));
}

GpHttpHeaderValue::GpHttpHeaderValue (const GpHttpHeaderValue& aValue):
elements(aValue.elements)
{
}

GpHttpHeaderValue::GpHttpHeaderValue (GpHttpHeaderValue&& aValue) noexcept:
elements(std::move(aValue.elements))
{
}

GpHttpHeaderValue::~GpHttpHeaderValue (void) noexcept
{
}

void    GpHttpHeaderValue::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(elements);
}

}//namespace GPlatform

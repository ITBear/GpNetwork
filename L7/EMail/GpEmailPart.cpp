#include "GpEmailPart.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpEmailPart, GP_MODULE_UUID)

GpEmailPart::GpEmailPart (void) noexcept
{
}

GpEmailPart::GpEmailPart
(
    const GpContentType::EnumT  aContentType,
    std::string&&               aData
) noexcept:
content_type(aContentType),
data(std::move(aData))
{
}

GpEmailPart::GpEmailPart
(
    const GpContentType::EnumT  aContentType,
    std::string_view            aData
):
content_type(aContentType),
data(aData)
{
}

GpEmailPart::~GpEmailPart (void) noexcept
{
}

void    GpEmailPart::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(content_type);
    PROP(data);
}

}//namespace GPlatform

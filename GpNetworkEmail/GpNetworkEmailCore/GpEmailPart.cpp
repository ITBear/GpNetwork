#include "GpEmailPart.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpEmailPart, GP_MODULE_UUID)

GpEmailPart::GpEmailPart (void) noexcept
{
}

GpEmailPart::GpEmailPart (const GpEmailPart& aPart):
GpReflectObject(aPart),
content_type(aPart.content_type),
data(aPart.data)
{
}

GpEmailPart::GpEmailPart (GpEmailPart&& aPart) noexcept:
GpReflectObject(std::move(aPart)),
content_type(std::move(aPart.content_type)),
data(std::move(aPart.data))
{
}

GpEmailPart::GpEmailPart
(
    const GpHttpContentType::EnumT  aContentType,
    std::string&&                   aData
) noexcept:
content_type(aContentType),
data(std::move(aData))
{
}

GpEmailPart::GpEmailPart
(
    const GpHttpContentType::EnumT  aContentType,
    std::string_view                aData
):
content_type(aContentType),
data(aData)
{
}

GpEmailPart::~GpEmailPart (void) noexcept
{
}

void    GpEmailPart::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(content_type);
    PROP(data);
}

}//namespace GPlatform

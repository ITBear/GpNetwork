#include "GpEmail.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpEmail, GP_MODULE_UUID)

GpEmail::GpEmail (void) noexcept
{
}

GpEmail::~GpEmail (void) noexcept
{
}

void    GpEmail::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(from);
    PROP(to);
    PROP(cc);
    PROP(subject);
    PROP(link_unsubscribe);
    PROP(parts);
}

}//namespace GPlatform

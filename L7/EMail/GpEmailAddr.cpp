#include "GpEmailAddr.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpEmailAddr, GP_MODULE_UUID)

GpEmailAddr::GpEmailAddr (void) noexcept
{
}

GpEmailAddr::GpEmailAddr
(
    std::string_view aAddr
):
addr(aAddr)
{
}

GpEmailAddr::GpEmailAddr
(
    std::string_view    aName,
    std::string_view    aAddr
):
name(aName),
addr(aAddr)
{
}

GpEmailAddr::~GpEmailAddr (void) noexcept
{
}

void    GpEmailAddr::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(name);
    PROP(addr);
}

}//namespace GPlatform

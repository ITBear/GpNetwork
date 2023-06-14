#include "GpEmailAddr.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpEmailAddr, GP_MODULE_UUID)

GpEmailAddr::GpEmailAddr (void) noexcept
{
}

GpEmailAddr::GpEmailAddr (const GpEmailAddr& aEmailAddr):
GpReflectObject(aEmailAddr),
name(aEmailAddr.name),
addr(aEmailAddr.addr)
{
}

GpEmailAddr::GpEmailAddr (GpEmailAddr&& aEmailAddr) noexcept:
GpReflectObject(std::move(aEmailAddr)),
name(std::move(aEmailAddr.name)),
addr(std::move(aEmailAddr.addr))
{
}

GpEmailAddr::GpEmailAddr
(
    std::u8string_view aAddr
):
addr(aAddr)
{
}

GpEmailAddr::GpEmailAddr
(
    std::u8string_view  aName,
    std::u8string_view  aAddr
):
name(aName),
addr(aAddr)
{
}

GpEmailAddr::~GpEmailAddr (void) noexcept
{
}

void    GpEmailAddr::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(name);
    PROP(addr);
}

}//namespace GPlatform

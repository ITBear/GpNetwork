#include "GpHttpResponseNoBodyDesc.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpHttpResponseNoBodyDesc, GP_MODULE_UUID)

GpHttpResponseNoBodyDesc::~GpHttpResponseNoBodyDesc (void) noexcept
{
}

void    GpHttpResponseNoBodyDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(http_version);
    PROP(code);
    PROP(headers);
}

}//namespace GPlatform

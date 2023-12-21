#include "GpUrlQuery.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpUrlQuery, GP_MODULE_UUID)

GpUrlQuery::~GpUrlQuery (void) noexcept
{
}

void    GpUrlQuery::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(params);
}

}//namespace GPlatform

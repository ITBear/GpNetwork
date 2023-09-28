#include "GpUrlAuthority.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectManager.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpUrlAuthority, GP_MODULE_UUID)

GpUrlAuthority::~GpUrlAuthority (void) noexcept
{
}

void    GpUrlAuthority::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(user_name);
    PROP(password);
    PROP(host);
    PROP(port);
}

}//namespace GPlatform

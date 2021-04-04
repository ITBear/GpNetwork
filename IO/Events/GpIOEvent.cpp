#include "GpIOEvent.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpIOEvent, GP_MODULE_UUID)

void    GpIOEvent::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(iIOEvents);
}

}//namespace GPlatform

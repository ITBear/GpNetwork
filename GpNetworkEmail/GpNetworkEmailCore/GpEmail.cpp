#include "GpEmail.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpEmail, GP_MODULE_UUID)

GpEmail::GpEmail (void) noexcept
{
}

GpEmail::GpEmail (const GpEmail& aEmail):
GpReflectObject(aEmail),
from(aEmail.from),
to(aEmail.to),  //TODO: add deep copy
cc(aEmail.cc),  //TODO: add deep copy
subject(aEmail.subject),
link_unsubscribe(aEmail.link_unsubscribe),
parts(aEmail.parts) //TODO: add deep copy
{
}

GpEmail::GpEmail (GpEmail&& aEmail) noexcept:
GpReflectObject(std::move(aEmail)),
from(std::move(aEmail.from)),
to(std::move(aEmail.to)),
cc(std::move(aEmail.cc)),
subject(std::move(aEmail.subject)),
link_unsubscribe(std::move(aEmail.link_unsubscribe)),
parts(std::move(aEmail.parts))
{
}

GpEmail::~GpEmail (void) noexcept
{
}

void    GpEmail::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(from);
    PROP(to);
    PROP(cc);
    PROP(subject);
    PROP(link_unsubscribe);
    PROP(parts);
}

}//namespace GPlatform

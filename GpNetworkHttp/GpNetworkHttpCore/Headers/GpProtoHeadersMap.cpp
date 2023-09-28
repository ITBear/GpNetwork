#include "GpProtoHeadersMap.hpp"
#include "../../../GpCore2/GpReflection/GpReflectManager.hpp"

namespace GPlatform {

REFLECT_IMPLEMENT(GpProtoHeadersMap, GP_MODULE_UUID)

GpProtoHeadersMap::~GpProtoHeadersMap (void) noexcept
{
}

void    GpProtoHeadersMap::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(headers);
}

std::vector<std::u8string>& GpProtoHeadersMap::GetOrCreateHeaders (std::u8string_view aName)
{
    std::u8string lowerName = GpUTF::SToLower(aName);
    auto iter = headers.find(lowerName);

    if (iter == headers.end())
    {
        return headers.emplace
        (
            std::move(lowerName),
            MakeSP<GpProtoHeaderValue>()
        ).first->second->elements;
    } else
    {
        return iter->second->elements;
    }
}

}//namespace GPlatform

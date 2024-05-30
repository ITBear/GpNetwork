#include "GpProtoHeadersMap.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpProtoHeadersMap, GP_MODULE_UUID)

GpProtoHeadersMap::~GpProtoHeadersMap (void) noexcept
{
}

void    GpProtoHeadersMap::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(headers);
}

std::vector<std::string>&   GpProtoHeadersMap::GetOrCreateHeaders (std::string_view aName)
{
    std::string lowerName = GpUTF::SToLower(aName);
    auto iter = headers.find(lowerName);

    if (iter == std::end(headers))
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

}// namespace GPlatform

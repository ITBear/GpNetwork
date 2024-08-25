#include "GpProtoHeadersMap.hpp"

namespace GPlatform {

GpProtoHeadersMap::~GpProtoHeadersMap (void) noexcept
{
}

GpProtoHeaderValue::ElementsT&  GpProtoHeadersMap::GetOrCreateHeaders (std::string_view aName)
{
    std::string lowerName = GpUTF::SToLower(aName);
    auto iter = iHeaders.find(lowerName);

    if (iter == std::end(iHeaders))
    {
        return iHeaders.emplace
        (
            std::move(lowerName),
            GpProtoHeaderValue{}
        ).first->second.iElements;
    } else
    {
        return iter->second.iElements;
    }
}

}// namespace GPlatform

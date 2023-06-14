#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectObject.hpp"
#include "../../../../GpCore2/GpReflection/GpReflectUtils.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpProtoHeaderValue final: public GpReflectObject
{
public:
    CLASS_DD(GpProtoHeaderValue)
    REFLECT_DECLARE(u8"3dacda2f-90ec-4305-80d3-729f955a4946"_uuid)

public:
                                GpProtoHeaderValue  (void) noexcept = default;
    inline                      GpProtoHeaderValue  (std::u8string_view aElement);
    inline                      GpProtoHeaderValue  (std::u8string&& aElement);
    inline                      GpProtoHeaderValue  (const GpProtoHeaderValue& aValue);
    inline                      GpProtoHeaderValue  (GpProtoHeaderValue&& aValue) noexcept;
    virtual                     ~GpProtoHeaderValue (void) noexcept override final;

public:
    std::vector<std::u8string>  elements;
};

GpProtoHeaderValue::GpProtoHeaderValue (std::u8string_view aElement)
{
    elements.emplace_back(std::move(aElement));
}

GpProtoHeaderValue::GpProtoHeaderValue (std::u8string&& aElement)
{
    elements.emplace_back(std::move(aElement));
}

GpProtoHeaderValue::GpProtoHeaderValue (const GpProtoHeaderValue& aValue):
GpReflectObject(aValue),
elements(GpReflectUtils::SCopyValue(aValue.elements))
{
}

GpProtoHeaderValue::GpProtoHeaderValue (GpProtoHeaderValue&& aValue) noexcept:
GpReflectObject(std::move(aValue)),
elements(std::move(aValue.elements))
{
}

}//namespace GPlatform

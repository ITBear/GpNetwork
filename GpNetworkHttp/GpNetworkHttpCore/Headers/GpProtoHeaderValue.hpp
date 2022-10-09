#pragma once

#include "../GpNetworkHttpCore_global.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpProtoHeaderValue final: public GpReflectObject
{
public:
    CLASS_DD(GpProtoHeaderValue)
    REFLECT_DECLARE("3dacda2f-90ec-4305-80d3-729f955a4946"_uuid)

public:
                                GpProtoHeaderValue  (void) noexcept;
                                GpProtoHeaderValue  (std::string_view aElement);
                                GpProtoHeaderValue  (std::string&& aElement);
                                GpProtoHeaderValue  (const GpProtoHeaderValue& aValue);
                                GpProtoHeaderValue  (GpProtoHeaderValue&& aValue) noexcept;
    virtual                     ~GpProtoHeaderValue (void) noexcept override final;

public:
    std::vector<std::string>    elements;
};

}//namespace GPlatform

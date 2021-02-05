#pragma once

#include "../../../GpNetwork_global.hpp"

namespace GPlatform {

class GPNETWORK_API GpProtoHeaderValue final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpProtoHeaderValue)
    TYPE_STRUCT_DECLARE("3dacda2f-90ec-4305-80d3-729f955a4946"_sv)

public:
    explicit                        GpProtoHeaderValue      (void) noexcept;
                                    GpProtoHeaderValue      (std::string_view aElement);
                                    GpProtoHeaderValue      (std::string&& aElement);
                                    GpProtoHeaderValue      (const GpProtoHeaderValue& aValue);
                                    GpProtoHeaderValue      (GpProtoHeaderValue&& aValue) noexcept;
    virtual                         ~GpProtoHeaderValue     (void) noexcept override final;

public:
    GpVector<std::string>           elements;
};

}//namespace GPlatform

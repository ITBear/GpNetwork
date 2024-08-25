#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCore_global.hpp>

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpProtoHeaderValue
{
public:
    CLASS_DD(GpProtoHeaderValue)

    using ElementsT = boost::container::small_vector<std::string, 2>;

public:
                                GpProtoHeaderValue  (void) noexcept = default;
    inline                      GpProtoHeaderValue  (const GpProtoHeaderValue& aValue);
    inline                      GpProtoHeaderValue  (GpProtoHeaderValue&& aValue) noexcept;
    inline                      GpProtoHeaderValue  (std::string_view aElement);
    inline                      GpProtoHeaderValue  (std::string aElement);
                                ~GpProtoHeaderValue (void) noexcept;

    inline GpProtoHeaderValue&  operator=           (const GpProtoHeaderValue& aValue);
    inline GpProtoHeaderValue&  operator=           (GpProtoHeaderValue&& aValue) noexcept;

public:
    ElementsT   iElements;
};

GpProtoHeaderValue::GpProtoHeaderValue (const GpProtoHeaderValue& aValue):
iElements{aValue.iElements}
{
}

GpProtoHeaderValue::GpProtoHeaderValue (GpProtoHeaderValue&& aValue) noexcept:
iElements{std::move(aValue.iElements)}
{
}

GpProtoHeaderValue::GpProtoHeaderValue (std::string_view aElement):
iElements{{std::string(aElement)}}
{
}

GpProtoHeaderValue::GpProtoHeaderValue (std::string aElement):
iElements{{std::move(aElement)}}
{
}

GpProtoHeaderValue& GpProtoHeaderValue::operator= (const GpProtoHeaderValue& aValue)
{
    iElements = aValue.iElements;

    return *this;
}

GpProtoHeaderValue& GpProtoHeaderValue::operator= (GpProtoHeaderValue&& aValue) noexcept
{
    iElements = std::move(aValue.iElements);

    return *this;
}

}// namespace GPlatform

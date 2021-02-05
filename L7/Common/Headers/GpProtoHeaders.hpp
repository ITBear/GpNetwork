#pragma once

#include "GpProtoHeaderValue.hpp"
#include "../Enums/GpEnums.hpp"

namespace GPlatform {

class GPNETWORK_API GpProtoHeaders: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpProtoHeaders)
    TYPE_STRUCT_DECLARE("88a01e4e-9105-4cd5-9990-4578ebb14b51"_sv)

public:
    explicit                                GpProtoHeaders      (void) noexcept;
                                            GpProtoHeaders      (const GpProtoHeaders& aHeaders);
                                            GpProtoHeaders      (GpProtoHeaders&& aHeaders) noexcept;
    virtual                                 ~GpProtoHeaders     (void) noexcept override;

    void                                    Set                 (const GpProtoHeaders& aHeaders);
    void                                    Set                 (GpProtoHeaders&& aHeaders) noexcept;

    void                                    Clear               (void);
    GpProtoHeaderValue::C::MapStr::SP&      Headers             (void) noexcept {return headers;}
    const GpProtoHeaderValue::C::MapStr::SP&
                                            Headers             (void) const noexcept {return headers;}

    GpProtoHeaders&                         Replace             (std::string        aName,
                                                                 std::string_view   aValue);
    GpProtoHeaders&                         Replace             (std::string        aName,
                                                                 std::string&&      aValue);
    GpProtoHeaders&                         Replace             (std::string        aName,
                                                                 const u_int_64     aValue);

    GpProtoHeaders&                         Add                 (std::string        aName,
                                                                 std::string_view   aValue);
    GpProtoHeaders&                         Add                 (std::string        aName,
                                                                 std::string&&      aValue);
    GpProtoHeaders&                         Add                 (std::string        aName,
                                                                 const u_int_64     aValue);
protected:
    template<typename T, typename V>
    GpProtoHeaders&                         Replace             (const typename T::EnumT    aType,
                                                                 std::string_view           aValue);
    template<typename T, typename V>
    GpProtoHeaders&                         Replace             (const typename T::EnumT    aType,
                                                                 std::string&&              aValue);
    template<typename T, typename V>
    GpProtoHeaders&                         Replace             (const typename T::EnumT    aType,
                                                                 const u_int_64             aValue);

    template<typename T, typename V>
    GpProtoHeaders&                         Add                 (const typename T::EnumT    aType,
                                                                 std::string_view           aValue);
    template<typename T, typename V>
    GpProtoHeaders&                         Add                 (const typename T::EnumT    aType,
                                                                 std::string&&              aValue);
    template<typename T, typename V>
    GpProtoHeaders&                         Add                 (const typename T::EnumT    aType,
                                                                 const u_int_64             aValue);
private:
    GpProtoHeaderValue::C::MapStr::SP       headers;

public:
    static const GpArray<std::string, GpContentType::SCount().As<size_t>()> sContentType;
    static const GpArray<std::string, GpCharset::SCount().As<size_t>()>     sCharset;
};

template<typename T, typename V>
GpProtoHeaders& GpProtoHeaders::Replace
(
    const typename T::EnumT aType,
    std::string_view        aValue
)
{
    return Replace<T, V>(aType, std::string(aValue));
}

template<typename T, typename V>
GpProtoHeaders& GpProtoHeaders::Replace
(
    const typename T::EnumT aType,
    std::string&&           aValue
)
{
    const std::string& headerName = V::sHeadersNames.at(size_t(aType));
    headers[headerName] = MakeSP<GpProtoHeaderValue>(std::move(aValue));

    return *this;
}

template<typename T, typename V>
GpProtoHeaders& GpProtoHeaders::Replace
(
    const typename T::EnumT aType,
    const u_int_64          aValue)
{
    return Replace<T, V>(aType, StrOps::SFromUI64(aValue));
}

template<typename T, typename V>
GpProtoHeaders& GpProtoHeaders::Add
(
    const typename T::EnumT aType,
    std::string_view        aValue
)
{
    return Add<T, V>(aType, std::string(aValue));
}

template<typename T, typename V>
GpProtoHeaders& GpProtoHeaders::Add
(
    const typename T::EnumT aType,
    std::string&&           aValue
)
{
    const std::string& headerName = V::sHeadersNames.at(size_t(aType));
    auto iter = headers.find(headerName);

    if (iter == headers.end())
    {
        return Replace<T, V>(aType, std::move(aValue));
    } else
    {
        iter->second->elements.emplace_back(std::move(aValue));
    }

    return *this;
}

template<typename T, typename V>
GpProtoHeaders& GpProtoHeaders::Add
(
    const typename T::EnumT aType,
    const u_int_64          aValue
)
{
    return Add<T, V>(aType, StrOps::SFromUI64(aValue));
}

}//namespace GPlatform

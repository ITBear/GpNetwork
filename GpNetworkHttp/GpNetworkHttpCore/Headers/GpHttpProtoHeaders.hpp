#pragma once

#include "GpProtoHeaderValue.hpp"
#include "GpHttpContentType.hpp"
#include "GpHttpCharset.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpProtoHeaders: public GpReflectObject
{
public:
    CLASS_DD(GpHttpProtoHeaders)
    REFLECT_DECLARE("88a01e4e-9105-4cd5-9990-4578ebb14b51"_uuid)

public:
    explicit                                GpHttpProtoHeaders  (void) noexcept;
    explicit                                GpHttpProtoHeaders  (const GpHttpProtoHeaders& aHeaders);
    explicit                                GpHttpProtoHeaders  (GpHttpProtoHeaders&& aHeaders) noexcept;
    virtual                                 ~GpHttpProtoHeaders (void) noexcept override;

    void                                    Set                 (const GpHttpProtoHeaders& aHeaders);
    void                                    Set                 (GpHttpProtoHeaders&& aHeaders) noexcept;

    void                                    Clear               (void);
    GpProtoHeaderValue::C::MapStr::SP&      Headers             (void) noexcept {return headers;}
    const GpProtoHeaderValue::C::MapStr::SP&
                                            Headers             (void) const noexcept {return headers;}

    GpHttpProtoHeaders&                     Replace             (std::string        aName,
                                                                 std::string_view   aValue);
    GpHttpProtoHeaders&                     Replace             (std::string        aName,
                                                                 std::string&&      aValue);
    GpHttpProtoHeaders&                     Replace             (std::string        aName,
                                                                 const u_int_64     aValue);

    GpHttpProtoHeaders&                     Add                 (std::string        aName,
                                                                 std::string_view   aValue);
    GpHttpProtoHeaders&                     Add                 (std::string        aName,
                                                                 std::string&&      aValue);
    GpHttpProtoHeaders&                     Add                 (std::string        aName,
                                                                 const u_int_64     aValue);
protected:
    template<typename T, typename V>
    GpHttpProtoHeaders&                     Replace             (const typename T::EnumT    aType,
                                                                 std::string_view           aValue);
    template<typename T, typename V>
    GpHttpProtoHeaders&                     Replace             (const typename T::EnumT    aType,
                                                                 std::string&&              aValue);
    template<typename T, typename V>
    GpHttpProtoHeaders&                     Replace             (const typename T::EnumT    aType,
                                                                 const u_int_64             aValue);

    template<typename T, typename V>
    GpHttpProtoHeaders&                     Add                 (const typename T::EnumT    aType,
                                                                 std::string_view           aValue);
    template<typename T, typename V>
    GpHttpProtoHeaders&                     Add                 (const typename T::EnumT    aType,
                                                                 std::string&&              aValue);
    template<typename T, typename V>
    GpHttpProtoHeaders&                     Add                 (const typename T::EnumT    aType,
                                                                 const u_int_64             aValue);
private:
    GpProtoHeaderValue::C::MapStr::SP       headers;

public:
    static const std::array<std::string, GpHttpContentType::SCount()>   sContentType;
    static const std::array<std::string, GpHttpCharset::SCount()>       sCharset;
};

template<typename T, typename V>
GpHttpProtoHeaders& GpHttpProtoHeaders::Replace
(
    const typename T::EnumT aType,
    std::string_view        aValue
)
{
    return Replace<T, V>(aType, std::string(aValue));
}

template<typename T, typename V>
GpHttpProtoHeaders& GpHttpProtoHeaders::Replace
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
GpHttpProtoHeaders& GpHttpProtoHeaders::Replace
(
    const typename T::EnumT aType,
    const u_int_64          aValue)
{
    return Replace<T, V>(aType, StrOps::SFromUI64(aValue));
}

template<typename T, typename V>
GpHttpProtoHeaders& GpHttpProtoHeaders::Add
(
    const typename T::EnumT aType,
    std::string_view        aValue
)
{
    return Add<T, V>(aType, std::string(aValue));
}

template<typename T, typename V>
GpHttpProtoHeaders& GpHttpProtoHeaders::Add
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
        iter->second.V().elements.emplace_back(std::move(aValue));
    }

    return *this;
}

template<typename T, typename V>
GpHttpProtoHeaders& GpHttpProtoHeaders::Add
(
    const typename T::EnumT aType,
    const u_int_64          aValue
)
{
    return Add<T, V>(aType, StrOps::SFromUI64(aValue));
}

}//namespace GPlatform

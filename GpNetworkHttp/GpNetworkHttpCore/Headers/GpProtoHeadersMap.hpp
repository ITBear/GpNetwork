#pragma once

#include "GpProtoHeaderValue.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpProtoHeadersMap: public GpReflectObject
{
public:
    CLASS_DD(GpProtoHeadersMap)
    REFLECT_DECLARE(u8"88a01e4e-9105-4cd5-9990-4578ebb14b51"_uuid)

public:
    inline                                      GpProtoHeadersMap   (void) noexcept;
    inline                                      GpProtoHeadersMap   (const GpProtoHeadersMap& aHeaders);
    inline                                      GpProtoHeadersMap   (GpProtoHeadersMap&& aHeaders) noexcept;
    virtual                                     ~GpProtoHeadersMap  (void) noexcept override;

    void                                        Clear               (void) {headers.clear();}
    GpProtoHeaderValue::C::MapStr::SP&          Headers             (void) noexcept {return headers;}
    const GpProtoHeaderValue::C::MapStr::SP&    Headers             (void) const noexcept {return headers;}

    inline void                                 Set                 (const GpProtoHeadersMap& aHeaders);
    inline void                                 Set                 (GpProtoHeadersMap&& aHeaders) noexcept;

    inline GpProtoHeadersMap&                   Set                 (std::u8string_view aName,
                                                                     std::u8string      aValue);
    inline GpProtoHeadersMap&                   Set                 (std::u8string_view aName,
                                                                     const u_int_64     aValue);
    template<EnumConcepts::IsEnum T>
    GpProtoHeadersMap&                          Set                 (const typename T::EnumT    aName,
                                                                     std::u8string              aValue);
    template<EnumConcepts::IsEnum T>
    GpProtoHeadersMap&                          Set                 (const typename T::EnumT    aName,
                                                                     const u_int_64             aValue);
    template<EnumConcepts::IsEnum T,
             EnumConcepts::IsEnum V>
    GpProtoHeadersMap&                          Set                 (const typename T::EnumT    aName,
                                                                     const typename V::EnumT    aValue);

    inline GpProtoHeadersMap&                   Add                 (std::u8string_view aName,
                                                                     std::u8string      aValue);
    inline GpProtoHeadersMap&                   Add                 (std::u8string_view aName,
                                                                     const u_int_64     aValue);
    template<EnumConcepts::IsEnum T>
    GpProtoHeadersMap&                          Add                 (const typename T::EnumT    aName,
                                                                     std::u8string              aValue);
    template<EnumConcepts::IsEnum T>
    GpProtoHeadersMap&                          Add                 (const typename T::EnumT    aName,
                                                                     const u_int_64             aValue);
    template<EnumConcepts::IsEnum T,
             EnumConcepts::IsEnum V>
    GpProtoHeadersMap&                          Add                 (const typename T::EnumT    aName,
                                                                     const typename V::EnumT    aValue);

    inline GpProtoHeaderValue::C::Opt::CRef     GetValues           (std::u8string_view aName) const noexcept;

    template<EnumConcepts::IsEnum T>
    GpProtoHeaderValue::C::Opt::CRef            GetValues           (const typename T::EnumT aName) const noexcept;

    inline std::optional<std::u8string_view>    GetValueStr         (std::u8string_view aName,
                                                                     const size_t       aElementId) const noexcept;
    template<EnumConcepts::IsEnum T>
    std::optional<std::u8string_view>           GetValueStr         (const typename T::EnumT    aName,
                                                                     const size_t               aElementId) const noexcept;
    template<EnumConcepts::IsEnum T,
             EnumConcepts::IsEnum V>
    typename std::optional<typename V::EnumT>   GetValueEnum        (const typename T::EnumT    aName,
                                                                     const size_t               aElementId);

private:
    std::vector<std::u8string>&                 GetOrCreateHeaders  (std::u8string_view aName);

private:
    GpProtoHeaderValue::C::MapStr::SP           headers;
};

GpProtoHeadersMap::GpProtoHeadersMap (void) noexcept
{
}

GpProtoHeadersMap::GpProtoHeadersMap (const GpProtoHeadersMap& aHeaders):
GpReflectObject(aHeaders),
headers(GpReflectUtils::SCopyValue(aHeaders.headers))
{
}

GpProtoHeadersMap::GpProtoHeadersMap (GpProtoHeadersMap&& aHeaders) noexcept:
GpReflectObject(std::move(aHeaders)),
headers(std::move(aHeaders.headers))
{
}

void    GpProtoHeadersMap::Set (const GpProtoHeadersMap& aHeaders)
{
    headers = aHeaders.headers;
}

void    GpProtoHeadersMap::Set (GpProtoHeadersMap&& aHeaders) noexcept
{
    headers = std::move(aHeaders.headers);
}

GpProtoHeadersMap&  GpProtoHeadersMap::Set
(
    std::u8string_view  aName,
    std::u8string       aValue
)
{
    GetOrCreateHeaders(aName) = {std::move(aValue)};
    return *this;
}

GpProtoHeadersMap&  GpProtoHeadersMap::Set
(
    std::u8string_view  aName,
    const u_int_64      aValue
)
{
    return Set
    (
        aName,
        StrOps::SFromUI64(aValue)
    );
}

template<EnumConcepts::IsEnum T>
GpProtoHeadersMap&  GpProtoHeadersMap::Set
(
    const typename T::EnumT aName,
    std::u8string           aValue
)
{
    return Set
    (
        GpProtoHeader_EnumToStr(aName),
        std::move(aValue)
    );
}

template<EnumConcepts::IsEnum T>
GpProtoHeadersMap&  GpProtoHeadersMap::Set
(
    const typename T::EnumT aName,
    const u_int_64          aValue
)
{
    return Set
    (
        GpProtoHeader_EnumToStr(aName),
        StrOps::SFromUI64(aValue)
    );
}

template<EnumConcepts::IsEnum T,
         EnumConcepts::IsEnum V>
GpProtoHeadersMap&  GpProtoHeadersMap::Set
(
    const typename T::EnumT aName,
    const typename V::EnumT aValue
)
{
    return Set
    (
        GpProtoHeader_EnumToStr(aName),
        std::u8string(GpProtoHeader_EnumToStr(aValue))
    );
}

GpProtoHeadersMap&  GpProtoHeadersMap::Add
(
    std::u8string_view  aName,
    std::u8string       aValue
)
{
    GetOrCreateHeaders(aName).emplace_back(std::move(aValue));
    return *this;
}

GpProtoHeadersMap&  GpProtoHeadersMap::Add
(
    std::u8string_view  aName,
    const u_int_64      aValue
)
{
    return Add
    (
        aName,
        StrOps::SFromUI64(aValue)
    );
}

template<EnumConcepts::IsEnum T>
GpProtoHeadersMap&  GpProtoHeadersMap::Add
(
    const typename T::EnumT aName,
    std::u8string           aValue
)
{
    return Add
    (
        GpProtoHeader_EnumToStr(aName),
        std::move(aValue)
    );
}

template<EnumConcepts::IsEnum T>
GpProtoHeadersMap&  GpProtoHeadersMap::Add
(
    const typename T::EnumT aName,
    const u_int_64          aValue
)
{
    return Add
    (
        GpProtoHeader_EnumToStr(aName),
        StrOps::SFromUI64(aValue)
    );
}

template<EnumConcepts::IsEnum T,
         EnumConcepts::IsEnum V>
GpProtoHeadersMap&  GpProtoHeadersMap::Add
(
    const typename T::EnumT aName,
    const typename V::EnumT aValue
)
{
    return Set
    (
        GpProtoHeader_EnumToStr(aName),
        std::u8string(GpProtoHeader_EnumToStr(aValue))
    );
}

GpProtoHeaderValue::C::Opt::CRef    GpProtoHeadersMap::GetValues (std::u8string_view aName) const noexcept
{
    std::u8string lowerName = GpUTF::SToLower(aName);

    const auto iter = headers.find(lowerName);

    if (iter == headers.end())
    {
        return std::nullopt;
    }

    return iter->second.V();
}

template<EnumConcepts::IsEnum T>
GpProtoHeaderValue::C::Opt::CRef    GpProtoHeadersMap::GetValues (const typename T::EnumT aName) const noexcept
{
    return GetValues(GpProtoHeader_EnumToStr(aName));
}

std::optional<std::u8string_view>   GpProtoHeadersMap::GetValueStr
(
    std::u8string_view  aName,
    const size_t        aElementId
) const noexcept
{
    GpProtoHeaderValue::C::Opt::CRef valueOptRef = GetValues(aName);

    if (!valueOptRef.has_value())
    {
        return std::nullopt;
    }

    const auto& elements = valueOptRef->get().elements;

    if (elements.size() >= aElementId)
    {
        return std::nullopt;
    }

    return elements[aElementId];
}

template<EnumConcepts::IsEnum T>
std::optional<std::u8string_view>   GpProtoHeadersMap::GetValueStr
(
    const typename T::EnumT aName,
    const size_t            aElementId
) const noexcept
{
    return GetValueStr(GpProtoHeader_EnumToStr(aName), aElementId);
}

template<EnumConcepts::IsEnum T,
         EnumConcepts::IsEnum V>
std::optional<typename V::EnumT>    GpProtoHeadersMap::GetValueEnum
(
    const typename T::EnumT aName,
    const size_t            aElementId
)
{
    std::optional<std::u8string_view> value = GetValueStr(GpProtoHeader_EnumToStr(aName), aElementId);

    if (!value.has_value())
    {
        return std::nullopt;
    }

    return GpProtoHeader_StrToEnum<V>(value.value());
}

}//namespace GPlatform

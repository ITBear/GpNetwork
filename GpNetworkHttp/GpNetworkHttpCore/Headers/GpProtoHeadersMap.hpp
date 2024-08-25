#pragma once

#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Headers/GpProtoHeaderValue.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpProtoHeadersMap
{
public:
    CLASS_DD(GpProtoHeadersMap)

    using HeadersT = GpProtoHeaderValue::C::SmallFlatMapStr<12>::Val;

public:
    inline                                      GpProtoHeadersMap   (void) noexcept;
    inline                                      GpProtoHeadersMap   (const GpProtoHeadersMap& aHeaders);
    inline                                      GpProtoHeadersMap   (GpProtoHeadersMap&& aHeaders) noexcept;
    virtual                                     ~GpProtoHeadersMap  (void) noexcept;

    void                                        Clear               (void) {iHeaders.clear();}
    bool                                        Empty               (void) const noexcept {return iHeaders.empty();}
    HeadersT&                                   Headers             (void) noexcept {return iHeaders;}
    const HeadersT&                             Headers             (void) const noexcept {return iHeaders;}

    inline void                                 Set                 (const GpProtoHeadersMap& aHeaders);
    inline void                                 Set                 (GpProtoHeadersMap&& aHeaders) noexcept;

    inline GpProtoHeadersMap&                   Set                 (std::string_view   aName,
                                                                     std::string        aValue);
    inline GpProtoHeadersMap&                   Set                 (std::string_view   aName,
                                                                     const u_int_64     aValue);
    template<EnumConcepts::IsEnum T>
    GpProtoHeadersMap&                          Set                 (const typename T::EnumT    aName,
                                                                     std::string                aValue);
    template<EnumConcepts::IsEnum T>
    GpProtoHeadersMap&                          Set                 (const typename T::EnumT    aName,
                                                                     const u_int_64             aValue);
    template<EnumConcepts::IsEnum T,
             EnumConcepts::IsEnum V>
    GpProtoHeadersMap&                          Set                 (const typename T::EnumT    aName,
                                                                     const typename V::EnumT    aValue);

    inline GpProtoHeadersMap&                   Add                 (std::string_view   aName,
                                                                     std::string        aValue);
    inline GpProtoHeadersMap&                   Add                 (std::string_view   aName,
                                                                     const u_int_64     aValue);
    template<EnumConcepts::IsEnum T>
    GpProtoHeadersMap&                          Add                 (const typename T::EnumT    aName,
                                                                     std::string                aValue);
    template<EnumConcepts::IsEnum T>
    GpProtoHeadersMap&                          Add                 (const typename T::EnumT    aName,
                                                                     const u_int_64             aValue);
    template<EnumConcepts::IsEnum T,
             EnumConcepts::IsEnum V>
    GpProtoHeadersMap&                          Add                 (const typename T::EnumT    aName,
                                                                     const typename V::EnumT    aValue);

    inline GpProtoHeaderValue::C::Opt::CRef     GetValues           (std::string_view   aName) const noexcept;

    template<EnumConcepts::IsEnum T>
    GpProtoHeaderValue::C::Opt::CRef            GetValues           (const typename T::EnumT aName) const noexcept;

    inline std::optional<std::string_view>      GetValueStr         (std::string_view   aName,
                                                                     const size_t       aElementId) const noexcept;
    template<EnumConcepts::IsEnum T>
    std::optional<std::string_view>             GetValueStr         (const typename T::EnumT    aName,
                                                                     const size_t               aElementId) const noexcept;
    template<EnumConcepts::IsEnum T,
             EnumConcepts::IsEnum V>
    typename std::optional<typename V::EnumT>   GetValueEnum        (const typename T::EnumT    aName,
                                                                     const size_t               aElementId) const noexcept;

private:
    GpProtoHeaderValue::ElementsT&              GetOrCreateHeaders  (std::string_view aName);

private:
    HeadersT                                    iHeaders;
};

GpProtoHeadersMap::GpProtoHeadersMap (void) noexcept
{
}

GpProtoHeadersMap::GpProtoHeadersMap (const GpProtoHeadersMap& aHeaders):
iHeaders{aHeaders.iHeaders}
{
}

GpProtoHeadersMap::GpProtoHeadersMap (GpProtoHeadersMap&& aHeaders) noexcept:
iHeaders{std::move(aHeaders.iHeaders)}
{
}

void    GpProtoHeadersMap::Set (const GpProtoHeadersMap& aHeaders)
{
    iHeaders = aHeaders.iHeaders;
}

void    GpProtoHeadersMap::Set (GpProtoHeadersMap&& aHeaders) noexcept
{
    iHeaders = std::move(aHeaders.iHeaders);
}

GpProtoHeadersMap&  GpProtoHeadersMap::Set
(
    std::string_view    aName,
    std::string         aValue
)
{
    GetOrCreateHeaders(aName) = {std::move(aValue)};

    return *this;
}

GpProtoHeadersMap&  GpProtoHeadersMap::Set
(
    std::string_view    aName,
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
    std::string             aValue
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
        std::string(GpProtoHeader_EnumToStr(aValue))
    );
}

GpProtoHeadersMap&  GpProtoHeadersMap::Add
(
    std::string_view    aName,
    std::string         aValue
)
{
    GetOrCreateHeaders(aName).emplace_back(std::move(aValue));
    return *this;
}

GpProtoHeadersMap&  GpProtoHeadersMap::Add
(
    std::string_view    aName,
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
    std::string             aValue
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
    return Add
    (
        GpProtoHeader_EnumToStr(aName),
        std::string(GpProtoHeader_EnumToStr(aValue))
    );
}

GpProtoHeaderValue::C::Opt::CRef    GpProtoHeadersMap::GetValues (std::string_view aName) const noexcept
{
    std::string lowerName = GpUTF::SToLower(aName);

    const auto iter = iHeaders.find(lowerName);

    if (iter == std::end(iHeaders))
    {
        return std::nullopt;
    }

    return iter->second;
}

template<EnumConcepts::IsEnum T>
GpProtoHeaderValue::C::Opt::CRef    GpProtoHeadersMap::GetValues (const typename T::EnumT aName) const noexcept
{
    return GetValues(GpProtoHeader_EnumToStr(aName));
}

std::optional<std::string_view> GpProtoHeadersMap::GetValueStr
(
    std::string_view    aName,
    const size_t        aElementId
) const noexcept
{
    GpProtoHeaderValue::C::Opt::CRef valueOptRef = GetValues(aName);

    if (!valueOptRef.has_value())
    {
        return std::nullopt;
    }

    const auto& elements = valueOptRef->get().iElements;

    if (aElementId >= std::size(elements))
    {
        return std::nullopt;
    }

    return elements[aElementId];
}

template<EnumConcepts::IsEnum T>
std::optional<std::string_view> GpProtoHeadersMap::GetValueStr
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
) const noexcept
{
    std::optional<std::string_view> value = GetValueStr(GpProtoHeader_EnumToStr(aName), aElementId);

    if (!value.has_value())
    {
        return std::nullopt;
    }

    return GpProtoHeader_StrToEnum(value.value(), typename V::EnumT{});
}

}// namespace GPlatform

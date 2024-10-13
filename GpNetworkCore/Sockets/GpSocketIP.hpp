#pragma once

#include <GpNetwork/GpNetworkCore/Sockets/GpSocketIPv.hpp>

namespace GPlatform {

// -------------------------------------- GpSocketIPv4 --------------------------------------------

class GP_NETWORK_CORE_API GpSocketIPv4
{
public:
    static constexpr GpSocketIPv::EnumT IPv(void) noexcept {return GpSocketIPv::IPv4;}

    using StorageT = std::array<std::byte, 4>;

public:
    inline                  GpSocketIPv4    (void) noexcept;
    inline                  GpSocketIPv4    (const GpSocketIPv4& aIPv4) noexcept;
    inline                  GpSocketIPv4    (GpSocketIPv4&& aIPv4) noexcept;

    GpSpanByteR             Data            (void) const noexcept {return iAddress;}
    GpSpanByteRW            Data            (void) noexcept {return iAddress;}
    std::string             ToStr           (void) const {return SToStr(*this);}

    static GpSocketIPv4     SFromStr        (std::string_view aStr);
    static std::string      SToStr          (const GpSocketIPv4& aIPv4);
    static GpSocketIPv4     SFromBytes      (GpSpanByteR aData);

private:
    StorageT                iAddress;
};

GpSocketIPv4::GpSocketIPv4 (void) noexcept
{
    iAddress.fill(std::byte(0));
}

GpSocketIPv4::GpSocketIPv4 (const GpSocketIPv4& aIPv4) noexcept:
iAddress{aIPv4.iAddress}
{
}

GpSocketIPv4::GpSocketIPv4 (GpSocketIPv4&& aIPv4) noexcept:
iAddress{std::move(aIPv4.iAddress)}
{
}

// -------------------------------------- GpSocketIPv6 --------------------------------------------

class GP_NETWORK_CORE_API GpSocketIPv6
{
public:
    static constexpr GpSocketIPv::EnumT IPv(void) noexcept {return GpSocketIPv::IPv6;}

    using StorageT = std::array<std::byte, 16>;

public:
    inline                  GpSocketIPv6    (void) noexcept;
    inline                  GpSocketIPv6    (const GpSocketIPv6& aIPv6) noexcept;
    inline                  GpSocketIPv6    (GpSocketIPv6&& aIPv6) noexcept;

    GpSpanByteR             Data            (void) const noexcept {return iAddress;}
    GpSpanByteRW            Data            (void) noexcept {return iAddress;}
    std::string             ToStr           (void) const {return SToStr(*this);}

    static GpSocketIPv6     SFromStr        (std::string_view aStr);
    static std::string      SToStr          (const GpSocketIPv6& aIPv6);
    static GpSocketIPv6     SFromBytes      (GpSpanByteR aData);

private:
    StorageT                iAddress;
};

GpSocketIPv6::GpSocketIPv6 (void) noexcept
{
    iAddress.fill(std::byte(0));
}

GpSocketIPv6::GpSocketIPv6 (const GpSocketIPv6& aIPv6) noexcept:
iAddress{aIPv6.iAddress}
{
}

GpSocketIPv6::GpSocketIPv6 (GpSocketIPv6&& aIPv6) noexcept:
iAddress{aIPv6.iAddress}
{
}

}// namespace GPlatform

// ---------------------------------------------- std -----------------------------------------------------
namespace std
{
    inline size_t size ([[maybe_unused]] const ::GPlatform::GpSocketIPv4& aSocketIPv4)
    {
        return 4;
    }

    inline const ::std::byte* data (const ::GPlatform::GpSocketIPv4& aSocketIPv4)
    {
        return aSocketIPv4.Data().Ptr();
    }

    inline ::std::byte* data (::GPlatform::GpSocketIPv4& aSocketIPv4)
    {
        return aSocketIPv4.Data().Ptr();
    }
}// namespace std

namespace std
{
    inline size_t size ([[maybe_unused]] const ::GPlatform::GpSocketIPv6& aSocketIPv6)
    {
        return 16;
    }

    inline const ::std::byte* data (const ::GPlatform::GpSocketIPv6& aSocketIPv6)
    {
        return aSocketIPv6.Data().Ptr();
    }

    inline ::std::byte* data (::GPlatform::GpSocketIPv6& aSocketIPv6)
    {
        return aSocketIPv6.Data().Ptr();
    }
}// namespace std

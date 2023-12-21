#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpUrlAuthority final: public GpReflectObject
{
public:
    CLASS_DD(GpUrlAuthority)
    REFLECT_DECLARE(u8"e54c96c2-2021-4283-04bd-9b742e90fd36"_uuid)

public:
                        GpUrlAuthority  (void) noexcept = default;
    inline              GpUrlAuthority  (const GpUrlAuthority& aAuthority);
    inline              GpUrlAuthority  (GpUrlAuthority&& aAuthority) noexcept;
    inline              GpUrlAuthority  (std::u8string  aUserName,
                                         std::u8string  aPassword,
                                         std::u8string  aHost,
                                         const u_int_16 aPort) noexcept;
                        ~GpUrlAuthority (void) noexcept;

    std::u8string_view  UserName        (void) const noexcept {return user_name;}
    std::u8string_view  Password        (void) const noexcept {return password;}
    std::u8string_view  Host            (void) const noexcept {return host;}
    u_int_16            Port            (void) const noexcept {return port;}

private:
    std::u8string       user_name;
    std::u8string       password;
    std::u8string       host;
    u_int_16            port = 0;
};

GpUrlAuthority::GpUrlAuthority (const GpUrlAuthority& aAuthority):
GpReflectObject(aAuthority),
user_name(GpReflectUtils::SCopyValue(aAuthority.user_name)),
password (GpReflectUtils::SCopyValue(aAuthority.password)),
host     (GpReflectUtils::SCopyValue(aAuthority.host)),
port     (GpReflectUtils::SCopyValue(aAuthority.port))
{
}

GpUrlAuthority::GpUrlAuthority (GpUrlAuthority&& aAuthority) noexcept:
GpReflectObject(std::move(aAuthority)),
user_name(std::move(aAuthority.user_name)),
password (std::move(aAuthority.password)),
host     (std::move(aAuthority.host)),
port     (std::move(aAuthority.port))
{
}

GpUrlAuthority::GpUrlAuthority
(
    std::u8string   aUserName,
    std::u8string   aPassword,
    std::u8string   aHost,
    const u_int_16  aPort
) noexcept:
user_name(std::move(aUserName)),
password (std::move(aPassword)),
host     (std::move(aHost)),
port     (std::move(aPort))
{
}

}//namespace GPlatform

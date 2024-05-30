#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../Headers/GpHttpHeaders.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpUrlAuthority final: public GpReflectObject
{
public:
    CLASS_DD(GpUrlAuthority)
    REFLECT_DECLARE("e54c96c2-2021-4283-04bd-9b742e90fd36"_uuid)

public:
                            GpUrlAuthority      (void) noexcept = default;
                            GpUrlAuthority      (const GpUrlAuthority& aAuthority);
                            GpUrlAuthority      (GpUrlAuthority&& aAuthority) noexcept;
                            GpUrlAuthority      (std::string    aUserName,
                                                 std::string    aPassword,
                                                 std::string    aHost,
                                                 const u_int_16 aPort) noexcept;
                            ~GpUrlAuthority     (void) noexcept;

    GpUrlAuthority&         operator=           (const GpUrlAuthority& aAuthority);
    GpUrlAuthority&         operator=           (GpUrlAuthority&& aAuthority) noexcept;

    void                    Clear               (void);
    void                    SetFromHeaders      (const GpHttpHeaders& aHeaders);

    std::string_view        UserName            (void) const noexcept {return user_name;}
    std::string_view        Password            (void) const noexcept {return password;}
    std::string_view        Host                (void) const noexcept {return host;}
    u_int_16                Port                (void) const noexcept {return port;}

    static std::string      SToString           (const GpUrlAuthority& aAuthority);
    static GpUrlAuthority   SFromString         (std::string_view aAuthority);

private:
    static void             SUserPasswordFromStr(std::string_view   aUserNameAndPasswordStr,
                                                 std::string&       aUserNameOut,
                                                 std::string&       aPasswordOut);
    static void             SHostPortFromStr    (std::string_view   aHostAndPortStr,
                                                 std::string&       aHostOut,
                                                 u_int_16&          aPortOut);

private:
    std::string             user_name;
    std::string             password;
    std::string             host;
    u_int_16                port = 0;
};

}// namespace GPlatform

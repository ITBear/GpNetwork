#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCore_global.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Headers/GpHttpHeaders.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpUrlAuthority
{
public:
    CLASS_DD(GpUrlAuthority)

public:
                            GpUrlAuthority      (void) noexcept = default;
                            GpUrlAuthority      (const GpUrlAuthority& aAuthority);
                            GpUrlAuthority      (GpUrlAuthority&& aAuthority) noexcept;
                            GpUrlAuthority      (std::string    aUserName,
                                                 std::string    aPassword,
                                                 std::string    aHost,
                                                 u_int_16       aPort) noexcept;
                            ~GpUrlAuthority     (void) noexcept;

    GpUrlAuthority&         operator=           (const GpUrlAuthority& aAuthority);
    GpUrlAuthority&         operator=           (GpUrlAuthority&& aAuthority) noexcept;

    void                    Clear               (void);
    void                    SetFromHeaders      (const GpHttpHeaders& aHeaders);

    std::string_view        UserName            (void) const noexcept {return iUserName;}
    std::string_view        Password            (void) const noexcept {return iPassword;}
    std::string_view        Host                (void) const noexcept {return iHost;}
    u_int_16                Port                (void) const noexcept {return iPort;}

    std::string             ToString            (void) const;
    static GpUrlAuthority   SFromString         (std::string_view aAuthority);

private:
    static void             SUserPasswordFromStr(std::string_view   aUserNameAndPasswordStr,
                                                 std::string&       aUserNameOut,
                                                 std::string&       aPasswordOut);
    static void             SHostPortFromStr    (std::string_view   aHostAndPortStr,
                                                 std::string&       aHostOut,
                                                 u_int_16&          aPortOut);

private:
    std::string             iUserName;
    std::string             iPassword;
    std::string             iHost;
    u_int_16                iPort = 0;
};

}// namespace GPlatform

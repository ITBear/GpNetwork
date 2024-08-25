#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Url/GpUrlAuthority.hpp>

namespace GPlatform {

GpUrlAuthority::GpUrlAuthority (const GpUrlAuthority& aAuthority):
iUserName{aAuthority.iUserName},
iPassword{aAuthority.iPassword},
iHost    {aAuthority.iHost},
iPort    {aAuthority.iPort}
{
}

GpUrlAuthority::GpUrlAuthority (GpUrlAuthority&& aAuthority) noexcept:
iUserName{std::move(aAuthority.iUserName)},
iPassword{std::move(aAuthority.iPassword)},
iHost    {std::move(aAuthority.iHost)},
iPort    {std::move(aAuthority.iPort)}
{
}

GpUrlAuthority::GpUrlAuthority
(
    std::string     aUserName,
    std::string     aPassword,
    std::string     aHost,
    const u_int_16  aPort
) noexcept:
iUserName{std::move(aUserName)},
iPassword{std::move(aPassword)},
iHost    {std::move(aHost)},
iPort    {std::move(aPort)}
{
}

GpUrlAuthority::~GpUrlAuthority (void) noexcept
{
}

GpUrlAuthority& GpUrlAuthority::operator= (const GpUrlAuthority& aAuthority)
{
    iUserName   = aAuthority.iUserName;
    iPassword   = aAuthority.iPassword;
    iHost       = aAuthority.iHost;
    iPort       = aAuthority.iPort;

    return *this;
}

GpUrlAuthority& GpUrlAuthority::operator= (GpUrlAuthority&& aAuthority) noexcept
{
    iUserName   = std::move(aAuthority.iUserName);
    iPassword   = std::move(aAuthority.iPassword);
    iHost       = std::move(aAuthority.iHost);
    iPort       = std::move(aAuthority.iPort);

    return *this;
}

void    GpUrlAuthority::Clear (void)
{
    iUserName.clear();
    iPassword.clear();
    iHost.clear();
    iPort = 0;
}

void    GpUrlAuthority::SetFromHeaders (const GpHttpHeaders& aHeaders)
{
    std::optional<std::string_view> hostStrOpt = aHeaders.GetValueStr<GpHttpHeaderType>(GpHttpHeaderType::HOST, 0);
    std::optional<std::string_view> authStrOpt = aHeaders.GetValueStr<GpHttpHeaderType>(GpHttpHeaderType::AUTHORIZATION, 0);

    if (hostStrOpt.has_value())
    {
        SHostPortFromStr(hostStrOpt.value(), iHost, iPort);
    }

    if (authStrOpt.has_value())
    {
        SUserPasswordFromStr(authStrOpt.value(), iUserName, iPassword);
    }
}

std::string GpUrlAuthority::ToString (void) const
{
    std::string str;
    str.reserve(64);

    std::string_view userName   = UserName();
    std::string_view password   = Password();
    std::string_view host       = Host();

    // userName, password
    if (!userName.empty())
    {
        str.append(userName);

        if (!password.empty())
        {
            str.append(":").append(password);
        }

        str.append("@");
    }

    // host
    str.append(host);

    // port
    if (Port() > 0)
    {
        str.append(":").append(std::to_string(Port()));
    }

    return str;
}

GpUrlAuthority  GpUrlAuthority::SFromString (std::string_view aAuthority)
{
    GpUrlAuthority authority;

    std::string_view userNameAndPassword;
    std::string_view hostAndPort;
    if (const auto pos = aAuthority.find("@"_sv); pos != std::string::npos)
    {
        userNameAndPassword = aAuthority.substr(0, pos);
        hostAndPort         = aAuthority.substr(pos + 1);
    } else
    {
        userNameAndPassword = "";
        hostAndPort         = aAuthority;
    }

    SUserPasswordFromStr
    (
        userNameAndPassword,
        authority.iUserName,
        authority.iPassword
    );

    SHostPortFromStr
    (
        hostAndPort,
        authority.iHost,
        authority.iPort
    );

    return authority;
}

void    GpUrlAuthority::SUserPasswordFromStr
(
    std::string_view    aUserNameAndPasswordStr,
    std::string&        aUserNameOut,
    std::string&        aPasswordOut
)
{
    if (aUserNameAndPasswordStr.empty())
    {
        return;
    }

    if (const auto pos = aUserNameAndPasswordStr.find(":"_sv); pos != std::string::npos)
    {
        aUserNameOut = aUserNameAndPasswordStr.substr(0, pos);
        aPasswordOut = aUserNameAndPasswordStr.substr(pos + 1);
    } else
    {
        aUserNameOut = aUserNameAndPasswordStr;
        aPasswordOut = "";
    }
}

void    GpUrlAuthority::SHostPortFromStr
(
    std::string_view    aHostAndPortStr,
    std::string&        aHostOut,
    u_int_16&           aPortOut
)
{
    if (const auto pos = aHostAndPortStr.find(":"_sv); pos != std::string::npos)
    {
        aHostOut = aHostAndPortStr.substr(0, pos);
        aPortOut = NumOps::SConvert<u_int_16>(StrOps::SToSI64(aHostAndPortStr.substr(pos + 1)));
    } else
    {
        aHostOut = aHostAndPortStr;
    }
}

}// namespace GPlatform

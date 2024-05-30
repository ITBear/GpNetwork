#include "GpUrlAuthority.hpp"

#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpUrlAuthority, GP_MODULE_UUID)

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
    std::string     aUserName,
    std::string     aPassword,
    std::string     aHost,
    const u_int_16  aPort
) noexcept:
user_name(std::move(aUserName)),
password (std::move(aPassword)),
host     (std::move(aHost)),
port     (std::move(aPort))
{
}

GpUrlAuthority::~GpUrlAuthority (void) noexcept
{
}

GpUrlAuthority& GpUrlAuthority::operator= (const GpUrlAuthority& aAuthority)
{
    user_name   = aAuthority.user_name;
    password    = aAuthority.password;
    host        = aAuthority.host;
    port        = aAuthority.port;

    return *this;
}

GpUrlAuthority& GpUrlAuthority::operator= (GpUrlAuthority&& aAuthority) noexcept
{
    user_name   = std::move(aAuthority.user_name);
    password    = std::move(aAuthority.password);
    host        = std::move(aAuthority.host);
    port        = std::move(aAuthority.port);

    return *this;
}

void    GpUrlAuthority::Clear (void)
{
    user_name.clear();
    password.clear();
    host.clear();
    port = 0;
}

void    GpUrlAuthority::SetFromHeaders (const GpHttpHeaders& aHeaders)
{
    std::optional<std::string_view> hostStrOpt = aHeaders.GetValueStr<GpHttpHeaderType>(GpHttpHeaderType::HOST, 0);
    std::optional<std::string_view> authStrOpt = aHeaders.GetValueStr<GpHttpHeaderType>(GpHttpHeaderType::AUTHORIZATION, 0);

    if (hostStrOpt.has_value())
    {
        SHostPortFromStr(hostStrOpt.value(), host, port);
    }

    if (authStrOpt.has_value())
    {
        SUserPasswordFromStr(authStrOpt.value(), user_name, password);
    }
}

std::string GpUrlAuthority::SToString (const GpUrlAuthority& aAuthority)
{
    std::string str;
    str.reserve(64);

    std::string_view user_name  = aAuthority.UserName();
    std::string_view password   = aAuthority.Password();
    std::string_view host       = aAuthority.Host();

    // user_name, password
    if (!user_name.empty())
    {
        str.append(user_name);

        if (!password.empty())
        {
            str.append(":").append(password);
        }

        str.append("@");
    }

    // host
    str.append(host);

    // port
    if (aAuthority.Port() > 0)
    {
        str.append(":").append(std::to_string(aAuthority.Port()));
    }

    return str;
}

GpUrlAuthority  GpUrlAuthority::SFromString (std::string_view aAuthority)
{
    GpUrlAuthority authority;

    std::string_view user_name_and_password;
    std::string_view host_and_port;
    if (const auto pos = aAuthority.find("@"_sv); pos != std::string::npos)
    {
        user_name_and_password  = aAuthority.substr(0, pos);
        host_and_port           = aAuthority.substr(pos + 1);
    } else
    {
        user_name_and_password  = "";
        host_and_port           = aAuthority;
    }

    SUserPasswordFromStr
    (
        user_name_and_password,
        authority.user_name,
        authority.password
    );

    SHostPortFromStr
    (
        host_and_port,
        authority.host,
        authority.port
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

void    GpUrlAuthority::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(user_name);
    PROP(password);
    PROP(host);
    PROP(port);
}

}// namespace GPlatform

#include "GpSmtpClient.hpp"
#include "../GpEmailEncoder.hpp"

namespace GPlatform {

GpSmtpClient::GpSmtpClient (void) noexcept
{
}

GpSmtpClient::~GpSmtpClient (void) noexcept
{
}

void    GpSmtpClient::SetSmtp
(
    std::string_view aSmtp
)
{
    iSmtp = aSmtp;
}

void    GpSmtpClient::SetAuth
(
    std::string_view aLogin,
    std::string_view aPassword
)
{
    iLogin      = aLogin;
    iPassword   = aPassword;
}

std::string GpSmtpClient::MakeBody
(
    const GpEmail&  aEmail,
    GpByteWriter&   aWriter
)
{
    GpEmailEncoder encoder(aEmail, aWriter);
    return encoder.Encode();
}

}//namespace GPlatform

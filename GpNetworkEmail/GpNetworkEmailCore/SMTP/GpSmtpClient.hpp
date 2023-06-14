#pragma once

#include "../GpEmail.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpSmtpClient
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSmtpClient)
    CLASS_DD(GpSmtpClient)

public:
                                    GpSmtpClient    (void) noexcept;
    virtual                         ~GpSmtpClient   (void) noexcept;

    void                            SetSmtp         (std::u8string_view aSmtp);
    void                            SetAuth         (std::u8string_view aLogin,
                                                     std::u8string_view aPassword);

    std::u8string_view              Smtp            (void) const noexcept {return iSmtp;}
    std::u8string_view              Login           (void) const noexcept {return iLogin;}
    std::u8string_view              Password        (void) const noexcept {return iPassword;}


    virtual std::u8string/*msg_id*/ Send            (const GpEmail& aEmail) = 0;

    virtual bool                    IsValid         (void) const noexcept = 0;

protected:
    std::u8string                   MakeBody        (const GpEmail& aEmail,
                                                     GpByteWriter&  aWriter);

private:
    std::u8string                   iSmtp;
    std::u8string                   iLogin;
    std::u8string                   iPassword;
};

}//namespace GPlatform

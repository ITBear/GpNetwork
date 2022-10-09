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

    void                            SetSmtp         (std::string_view aSmtp);
    void                            SetAuth         (std::string_view aLogin,
                                                     std::string_view aPassword);

    std::string_view                Smtp            (void) const noexcept {return iSmtp;}
    std::string_view                Login           (void) const noexcept {return iLogin;}
    std::string_view                Password        (void) const noexcept {return iPassword;}


    virtual std::string/*msg_id*/   Send            (const GpEmail& aEmail) = 0;

    virtual bool                    IsValid         (void) const noexcept = 0;

protected:
    std::string                     MakeBody        (const GpEmail& aEmail,
                                                     GpByteWriter&  aWriter);

private:
    std::string                     iSmtp;
    std::string                     iLogin;
    std::string                     iPassword;
};

}//namespace GPlatform

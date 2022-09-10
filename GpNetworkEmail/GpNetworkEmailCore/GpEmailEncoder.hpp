#pragma once

#include "GpEmail.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpEmailEncoder
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpEmailEncoder)

public:
                        GpEmailEncoder  (const GpEmail& aEmail,
                                         GpByteWriter&  aWriter) noexcept;
                        ~GpEmailEncoder (void) noexcept;

    //return message-id
    std::string         Encode          (void);

private:
    void                Mime            (void);
    void                Date            (void);
    std::string         MessageID       (void);
    void                From            (void);
    void                To              (void);
    void                Cc              (void);
    void                Subject         (void);
    void                LinkUnsubscribe (void);
    void                Part            (const GpEmailPart& aPart,
                                         std::string_view   aBoundary);
    void                Parts           (void);
    void                Addr            (const GpEmailAddr& aAddr);
    void                Addrs           (const GpEmailAddr::C::Vec::SP& aAddr);
    void                WriteHeaderBegin(const GpEmailHeaderType::EnumT aHeader);
    void                BraceStr        (std::string_view aStr);
    void                EscapeStrBase64 (std::string_view aStr);

private:
    const GpEmail&      iEmail;
    GpByteWriter&       iWriter;
    bool                iIsFirstHeader = true;
};

}//namespace GPlatform

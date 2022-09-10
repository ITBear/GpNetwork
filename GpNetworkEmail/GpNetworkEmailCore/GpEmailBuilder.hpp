#pragma once

#include "GpEmail.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpEmailBuilder
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpEmailBuilder)

public:
                        GpEmailBuilder  (void) noexcept;
                        ~GpEmailBuilder (void) noexcept;

    GpEmail::SP         Build           (void) noexcept {return iEmail;}

    GpEmailBuilder&     From            (std::string_view aName, std::string_view aAddress);
    GpEmailBuilder&     From            (std::string_view aAddress);
    GpEmailBuilder&     To              (std::string_view aName, std::string_view aAddress);
    GpEmailBuilder&     To              (std::string_view aAddress);
    GpEmailBuilder&     Cc              (std::string_view aName, std::string_view aAddress);
    GpEmailBuilder&     Cc              (std::string_view aAddress);
    GpEmailBuilder&     Subject         (std::string_view aSubject);
    GpEmailBuilder&     LinkUnsubscribe (std::string_view aLink);
    GpEmailBuilder&     Part            (GpEmailPart::SP aPart);
    GpEmailBuilder&     Part            (const GpHttpContentType::EnumT aContentType,
                                         std::string&&                  aData);
    GpEmailBuilder&     Part            (const GpHttpContentType::EnumT aContentType,
                                         std::string_view               aData);

private:
    GpEmail&            _Email          (void);

private:
    GpEmail::SP         iEmail;
};

}//namespace GPlatform

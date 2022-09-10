#pragma once

#include "GpEmailHeaders.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpEmailPart final: public GpReflectObject
{
public:
    CLASS_DECLARE_DEFAULTS(GpEmailPart)
    REFLECT_DECLARE("8a9be760-5399-4695-b0f6-463ffc0320fa"_uuid)

public:
                                GpEmailPart         (void) noexcept;
                                GpEmailPart         (const GpEmailPart& aPart);
                                GpEmailPart         (GpEmailPart&& aPart) noexcept;
                                GpEmailPart         (const GpHttpContentType::EnumT aContentType,
                                                     std::string&&                  aData) noexcept;
                                GpEmailPart         (const GpHttpContentType::EnumT aContentType,
                                                     std::string_view               aData);
    virtual                     ~GpEmailPart        (void) noexcept override final;

public:
    GpHttpContentType           content_type;
    std::string                 data;
};

}//namespace GPlatform

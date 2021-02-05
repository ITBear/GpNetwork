#pragma once

#include "GpEmailHeaders.hpp"

namespace GPlatform {

class GPNETWORK_API GpEmailPart final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpEmailPart)
    TYPE_STRUCT_DECLARE("8a9be760-5399-4695-b0f6-463ffc0320fa"_sv)

public:
                                GpEmailPart         (void) noexcept;
                                GpEmailPart         (const GpContentType::EnumT aContentType,
                                                     std::string&&              aData) noexcept;
                                GpEmailPart         (const GpContentType::EnumT aContentType,
                                                     std::string_view           aData);
    virtual                     ~GpEmailPart        (void) noexcept override final;

public:
    GpContentType               content_type;
    std::string                 data;
};

}//namespace GPlatform

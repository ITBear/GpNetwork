#pragma once

#include "GpEmailHeaders.hpp"
#include "GpEmailPart.hpp"
#include "GpEmailAddr.hpp"

namespace GPlatform {

class GP_NETWORK_EMAIL_CORE_API GpEmail final: public GpReflectObject
{
public:
    CLASS_DD(GpEmail)
    REFLECT_DECLARE(u8"1e137d1b-8a9b-486f-a3d6-5708a02b7360"_uuid)

public:
                                    GpEmail             (void) noexcept;
                                    GpEmail             (const GpEmail& aEmail);
                                    GpEmail             (GpEmail&& aEmail) noexcept;
    virtual                         ~GpEmail            (void) noexcept override final;

public:
    GpEmailAddr                     from;
    GpEmailAddr::C::Vec::SP         to;
    GpEmailAddr::C::Vec::SP         cc;
    std::u8string                   subject;
    std::u8string                   link_unsubscribe;
    GpEmailPart::C::Vec::SP         parts;
};

}//namespace GPlatform

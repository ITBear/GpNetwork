#pragma once

#include "GpEmailHeaders.hpp"
#include "GpEmailPart.hpp"
#include "GpEmailAddr.hpp"

namespace GPlatform {

class GPNETWORK_API GpEmail final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpEmail)
    TYPE_STRUCT_DECLARE("1e137d1b-8a9b-486f-a3d6-5708a02b7360"_sv)

public:
                                    GpEmail             (void) noexcept;
    virtual                         ~GpEmail            (void) noexcept override final;

public:
    GpEmailAddr                     from;
    GpEmailAddr::C::Vec::SP         to;
    GpEmailAddr::C::Vec::SP         cc;
    std::string                     subject;
    std::string                     link_unsubscribe;
    GpEmailPart::C::Vec::SP         parts;
};

}//namespace GPlatform

#pragma once

#include "GpEmailHeaders.hpp"

namespace GPlatform {

class GPNETWORK_API GpEmailAddr final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpEmailAddr)
    TYPE_STRUCT_DECLARE("464b10e2-1169-4c1e-9782-f4a71838840e"_sv)

public:
                                GpEmailAddr         (void) noexcept;
                                GpEmailAddr         (std::string_view   aAddr);
                                GpEmailAddr         (std::string_view   aName,
                                                     std::string_view   aAddr);
    virtual                     ~GpEmailAddr        (void) noexcept override final;

public:
    std::string                 name;
    std::string                 addr;
};

}//namespace GPlatform

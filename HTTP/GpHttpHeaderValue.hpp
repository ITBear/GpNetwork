#pragma once

#include "GpHttpHeaderType.hpp"
#include "GpHttpContentType.hpp"
#include "GpHttpCharset.hpp"
#include "GpHttpConnectionFlag.hpp"
#include "GpHttpCacheControl.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpHeaderValue final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpHttpHeaderValue)
    TYPE_STRUCT_DECLARE("0b24dc63-ea71-4b61-bd72-e377fa499330"_sv)

public:
                                    GpHttpHeaderValue       (void) noexcept;
                                    GpHttpHeaderValue       (std::string&& aElement);
                                    GpHttpHeaderValue       (const GpHttpHeaderValue& aValue);
                                    GpHttpHeaderValue       (GpHttpHeaderValue&& aValue) noexcept;
    virtual                         ~GpHttpHeaderValue      (void) noexcept override final;

public:
    GpVector<std::string>           elements;
};

}//namespace GPlatform

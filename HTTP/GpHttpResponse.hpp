#pragma once

#include "GpHttpResponseCode.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpResponse
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpResponse)
    CLASS_DECLARE_DEFAULTS(GpHttpResponse)

    using CodeT     = GpHttpResponseCode;
    using CodeTE    = CodeT::EnumT;

public:
                                GpHttpResponse  (void) noexcept;
                                GpHttpResponse  (CodeTE             aCode,
                                                 std::string_view   aContentType,
                                                 GpBytesArray&&     aBody);
                                ~GpHttpResponse (void) noexcept;

    CodeTE                      Code            (void) const noexcept {return iCode;}
    std::string_view            ContentType     (void) const noexcept {return iContentType;}
    const GpBytesArray&         Body            (void) const noexcept {return iBody;}
    GpBytesArray&               Body            (void) noexcept {return iBody;}

private:
    CodeTE                      iCode;
    std::string                 iContentType;
    GpBytesArray                iBody;
};

}//namespace GPlatform

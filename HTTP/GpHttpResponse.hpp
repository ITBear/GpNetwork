#pragma once

#include "GpHttpVersion.hpp"
#include "GpHttpResponseCode.hpp"
#include "GpHttpException.hpp"
#include "GpHttpHeaders.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpResponse final: public GpTypeStructBase
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpResponse)
    CLASS_DECLARE_DEFAULTS(GpHttpResponse)
    TYPE_STRUCT_DECLARE("1c48e524-5261-4250-aa72-76d3401cc607"_sv)

    using HttpVersionT      = GpHttpVersion;
    using HttpVersionTE     = HttpVersionT::EnumT;

    using CodeT             = GpHttpResponseCode;
    using CodeTE            = CodeT::EnumT;

public:
                            GpHttpResponse      (void) noexcept;
                            GpHttpResponse      (const CodeTE           aCode,
                                                 const GpHttpHeaders&   aHeaders);
                            GpHttpResponse      (const CodeTE           aCode,
                                                 GpHttpHeaders&&        aHeaders) noexcept;
                            GpHttpResponse      (const CodeTE           aCode,
                                                 const GpHttpHeaders&   aHeaders,
                                                 GpBytesArray&&         aBody);
                            GpHttpResponse      (const CodeTE           aCode,
                                                 GpHttpHeaders&&        aHeaders,
                                                 GpBytesArray&&         aBody) noexcept;
                            ~GpHttpResponse     (void) noexcept;

    void                    SetFromException    (const GpHttpException& aHttpEx);
    void                    SetFromException    (const GpException& aEx);
    void                    SetFromException    (const std::exception& aEx);

public:
    HttpVersionT            http_version    = HttpVersionT::HTTP_1_1;
    CodeT                   code            = CodeT::INTERNAL_SERVER_ERROR_500;
    GpHttpHeaders           headers;
    GpBytesArray            body;
};

}//namespace GPlatform

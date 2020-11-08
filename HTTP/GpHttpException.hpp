#pragma once

#include "GpHttpResponseCode.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpException final: public GpException
{
public:
    using CodeT     = GpHttpResponseCode;
    using CodeTE    = CodeT::EnumT;

private:
                            GpHttpException     (void) noexcept = delete;
                            GpHttpException     (const GpHttpException& aException) noexcept = delete;

    GpHttpException&        operator=           (const GpHttpException& aException) noexcept = delete;
    GpHttpException&        operator=           (GpHttpException&& aException) noexcept = delete;

public:
                            GpHttpException     (GpHttpException&& aException) noexcept;
                            GpHttpException     (CodeTE                 aCode,
                                                 std::string_view       aMsg,
                                                 const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    virtual                 ~GpHttpException    (void) noexcept override final;

    CodeTE                  Code                (void) const noexcept {return iCode;}

private:
    const CodeTE            iCode;
};

#define THROW_HTTP(CODE, MSG)                       throw GpHttpException((CODE), (MSG))
#define THROW_HTTP_COND_CHECK(COND, CODE)           if (!(COND)) throw GpHttpException((CODE), ("Condition not met: "#COND))
#define THROW_HTTP_COND_CHECK_M(COND, CODE, MSG)    if (!(COND)) throw GpHttpException((CODE), (MSG))

}//namespace GPlatform

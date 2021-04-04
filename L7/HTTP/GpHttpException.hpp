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

[[noreturn]] inline void    THROW_HTTP
(
    GpHttpResponseCode::EnumT   aHttpCode,
    std::string_view            aMsg,
    const SourceLocationT&      aSourceLocation = SourceLocationT::current()
)
{
    throw GpHttpException(aHttpCode, aMsg, aSourceLocation);
}

inline void THROW_HTTP_COND
(
    const bool                  aCondition,
    GpHttpResponseCode::EnumT   aHttpCode,
    std::string_view            aMsg,
    const SourceLocationT&      aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition)
    {
        throw GpHttpException(aHttpCode, aMsg, aSourceLocation);
    }
}

inline void THROW_HTTP_COND
(
    const bool                  aCondition,
    GpHttpResponseCode::EnumT   aHttpCode,
    ThrowMsgGenT                aMsgGenFn,
    const SourceLocationT&      aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition)
    {
        throw GpHttpException(aHttpCode, aMsgGenFn(), aSourceLocation);
    }
}

}//namespace GPlatform

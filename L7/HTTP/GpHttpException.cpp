#include "GpHttpException.hpp"

namespace GPlatform {

GpHttpException::GpHttpException (GpHttpException&& aException) noexcept:
GpException(std::move(aException)),
iCode(std::move(aException.iCode))
{
}

GpHttpException::GpHttpException (CodeTE                    aCode,
                                  std::string_view          aMsg,
                                  const SourceLocationT&    aSourceLocation) noexcept:
GpException(aMsg, aSourceLocation),
iCode(aCode)
{
}

GpHttpException::~GpHttpException (void) noexcept
{
}

}//namespace GPlatform

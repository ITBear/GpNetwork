#pragma once

#include "../Headers/GpHttpResponseCode.hpp"

namespace GPlatform {

using GpHttpExceptionCode = GpHttpResponseCode;

EXCEPTION_CODE(GP_NETWORK_HTTP_CORE_API, GpHttpException, HTTP)

}//namespace GPlatform

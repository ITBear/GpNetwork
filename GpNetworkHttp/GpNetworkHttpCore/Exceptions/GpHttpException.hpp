#pragma once

#include "../Enums/GpHttpResponseCode.hpp"

#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptionCode.hpp>

namespace GPlatform {

using GpHttpExceptionCode = GpHttpResponseCode;

EXCEPTION_CODE(GP_NETWORK_HTTP_CORE_API, GpHttpException, HTTP)

}//namespace GPlatform

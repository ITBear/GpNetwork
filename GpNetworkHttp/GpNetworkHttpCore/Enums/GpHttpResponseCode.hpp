#pragma once

#include "../GpNetworkHttpCore_global.hpp"

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpResponseCode,
    CONTINUE_100                        = 100,
    SWITCHING_PROTOCOLS_101             = 101,
    PROCESSING_102                      = 102,
    EARLY_HINTS_103                     = 103,
    OK_200                              = 200,
    CREATED_201                         = 201,
    ACCEPTED_202                        = 202,
    NON_AUTHORITATIVE_INFORMATION_203   = 203,
    NO_CONTENT_204                      = 204,
    RESET_CONTENT_205                   = 205,
    PARTIAL_CONTENT_206                 = 206,
    MULTIPLE_CHOICES_300                = 300,
    MOVED_PERMANENTLY_301               = 301,
    FOUND_302                           = 302,
    SEE_OTHER_303                       = 303,
    NOT_MODIFIED_304                    = 304,
    USE_PROXY_305                       = 305,
    SWITCH_PROXY_306                    = 306,
    TEMPORARY_REDIRECT_307              = 307,
    PERMANENT_REDIRECT_308              = 308,
    BAD_REQUEST_400                     = 400,
    UNAUTHORIZED_401                    = 401,
    PAYMENT_REQUIRED_402                = 402,
    FORBIDDEN_403                       = 403,
    NOT_FOUND_404                       = 404,
    METHOD_NOT_ALLOWED_405              = 405,
    NOT_ACCEPTABLE_406                  = 406,
    PROXY_AUTHENTICATION_REQUIRED_407   = 407,
    REQUEST_TIMEOUT_408                 = 408,
    CONFLICT_409                        = 409,
    GONE_410                            = 410,
    LENGTH_REQUIRED_411                 = 411,
    PRECONDITION_FAILED_412             = 412,
    PAYLOAD_TOO_LARGE_413               = 413,
    URI_TOO_LONG_414                    = 414,
    UNSUPPORTED_MEDIA_TYPE_415          = 415,
    RANGE_NOT_SATISFIABLE_416           = 416,
    EXPECTATION_FAILED_417              = 417,
    INTERNAL_SERVER_ERROR_500           = 500,
    NOT_IMPLEMENTED_501                 = 501,
    BAD_GATEWAY_502                     = 502,
    SERVICE_UNAVAILABLE_503             = 503,
    GATEWAY_TIMEOUT_504                 = 504,
    HTTP_VERSION_NOT_SUPPORTED_505      = 505,
    VARIANT_ALSO_NEGOTIATES_506         = 506
);

std::string_view    SGpHttpResponseCode_ToStr (GpHttpResponseCode::EnumT aCode);

}// namespace GPlatform

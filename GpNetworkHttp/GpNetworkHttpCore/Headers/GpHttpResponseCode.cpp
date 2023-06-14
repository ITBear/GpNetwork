#include "GpHttpResponseCode.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpResponseCode)

GpHttpResponseCode::EnumT   GpHttpResponseCodeUtils::SFromId (const size_t aCodeId)
{
    switch (aCodeId)
    {
        case 100: return GpHttpResponseCode::CONTINUE_100;
        case 101: return GpHttpResponseCode::SWITCHING_PROTOCOLS_101;
        case 102: return GpHttpResponseCode::PROCESSING_102;
        case 103: return GpHttpResponseCode::EARLY_HINTS_103;
        case 200: return GpHttpResponseCode::OK_200;
        case 201: return GpHttpResponseCode::CREATED_201;
        case 202: return GpHttpResponseCode::ACCEPTED_202;
        case 203: return GpHttpResponseCode::NON_AUTHORITATIVE_INFORMATION_203;
        case 204: return GpHttpResponseCode::NO_CONTENT_204;
        case 205: return GpHttpResponseCode::RESET_CONTENT_205;
        case 206: return GpHttpResponseCode::PARTIAL_CONTENT_206;
        case 300: return GpHttpResponseCode::MULTIPLE_CHOICES_300;
        case 301: return GpHttpResponseCode::MOVED_PERMANENTLY_301;
        case 302: return GpHttpResponseCode::FOUND_302;
        case 303: return GpHttpResponseCode::SEE_OTHER_303;
        case 304: return GpHttpResponseCode::NOT_MODIFIED_304;
        case 305: return GpHttpResponseCode::USE_PROXY_305;
        case 306: return GpHttpResponseCode::SWITCH_PROXY_306;
        case 307: return GpHttpResponseCode::TEMPORARY_REDIRECT_307;
        case 308: return GpHttpResponseCode::PERMANENT_REDIRECT_308;
        case 400: return GpHttpResponseCode::BAD_REQUEST_400;
        case 401: return GpHttpResponseCode::UNAUTHORIZED_401;
        case 402: return GpHttpResponseCode::PAYMENT_REQUIRED_402;
        case 403: return GpHttpResponseCode::FORBIDDEN_403;
        case 404: return GpHttpResponseCode::NOT_FOUND_404;
        case 405: return GpHttpResponseCode::METHOD_NOT_ALLOWED_405;
        case 406: return GpHttpResponseCode::NOT_ACCEPTABLE_406;
        case 407: return GpHttpResponseCode::PROXY_AUTHENTICATION_REQUIRED_407;
        case 408: return GpHttpResponseCode::REQUEST_TIMEOUT_408;
        case 409: return GpHttpResponseCode::CONFLICT_409;
        case 410: return GpHttpResponseCode::GONE_410;
        case 411: return GpHttpResponseCode::LENGTH_REQUIRED_411;
        case 412: return GpHttpResponseCode::PRECONDITION_FAILED_412;
        case 413: return GpHttpResponseCode::PAYLOAD_TOO_LARGE_413;
        case 414: return GpHttpResponseCode::URI_TOO_LONG_414;
        case 415: return GpHttpResponseCode::UNSUPPORTED_MEDIA_TYPE_415;
        case 416: return GpHttpResponseCode::RANGE_NOT_SATISFIABLE_416;
        case 417: return GpHttpResponseCode::EXPECTATION_FAILED_417;
        case 500: return GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
        case 501: return GpHttpResponseCode::NOT_IMPLEMENTED_501;
        case 502: return GpHttpResponseCode::BAD_GATEWAY_502;
        case 503: return GpHttpResponseCode::SERVICE_UNAVAILABLE_503;
        case 504: return GpHttpResponseCode::GATEWAY_TIMEOUT_504;
        case 505: return GpHttpResponseCode::HTTP_VERSION_NOT_SUPPORTED_505;
        case 506: return GpHttpResponseCode::VARIANT_ALSO_NEGOTIATES_506;
        default:
        {
            THROW_GP(u8"Unknown code "_sv + aCodeId);
        }
    }
}

}//namespace GPlatform

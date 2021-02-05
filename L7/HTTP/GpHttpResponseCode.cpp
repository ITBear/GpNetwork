#include "GpHttpResponseCode.hpp"

namespace GPlatform {

GP_ENUM_IMPL(GpHttpResponseCode);

GpHttpResponseCode::EnumT   GpHttpResponseCodeUtils::SFromId (const size_t aCodeId)
{
    switch (aCodeId)
    {
        case 100: return GpHttpResponseCode::CONTINUE_100;
        case 101: return GpHttpResponseCode::SWITCHING_PROTOCOLS_101;
        case 200: return GpHttpResponseCode::OK_200;
        case 400: return GpHttpResponseCode::BAD_REQUEST_400;
        case 401: return GpHttpResponseCode::UNAUTHORIZED_401;
        case 402: return GpHttpResponseCode::PAYMENT_REQUIRED_402;
        case 403: return GpHttpResponseCode::FORBIDDEN_403;
        case 404: return GpHttpResponseCode::NOT_FOUND_404;
        case 405: return GpHttpResponseCode::METHOD_NOT_ALLOWED_405;
        case 406: return GpHttpResponseCode::NOT_ACCEPTABLE_406;
        case 500: return GpHttpResponseCode::INTERNAL_SERVER_ERROR_500;
        default:
        {
            THROW_GPE("Unknown code "_sv + aCodeId);
        }
    }
}

}//namespace GPlatform

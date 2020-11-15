#pragma once

#include "../GpNetwork_global.hpp"

namespace GPlatform {

GP_ENUM(GPNETWORK_API, GpHttpResponseCode,
    CONTINUE_100,
    SWITCHING_PROTOCOLS_101,
    OK_200,
    BAD_REQUEST_400,
    UNAUTHORIZED_401,
    PAYMENT_REQUIRED_402,
    FORBIDDEN_403,
    NOT_FOUND_404,
    METHOD_NOT_ALLOWED_405,
    NOT_ACCEPTABLE_406,
    INTERNAL_SERVER_ERROR_500
);

class GPNETWORK_API GpHttpResponseCodeUtils
{
public:
    static GpHttpResponseCode::EnumT    SFromId (const size_t aCodeId);
};

}//namespace GPlatform

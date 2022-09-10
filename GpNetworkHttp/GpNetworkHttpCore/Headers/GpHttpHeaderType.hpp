#pragma once

#include "../GpNetworkHttpCore_global.hpp"

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpHeaderType,
    CONTENT_TYPE,
    CONTENT_LENGTH,
    CONNECTION,
    CACHE_CONTROL,
    AUTHORIZATION
);

}//namespace GPlatform

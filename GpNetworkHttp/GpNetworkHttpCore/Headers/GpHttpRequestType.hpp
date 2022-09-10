#pragma once

#include "../GpNetworkHttpCore_global.hpp"

namespace GPlatform {

GP_ENUM(GP_NETWORK_HTTP_CORE_API, GpHttpRequestType,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
);

}//namespace GPlatform

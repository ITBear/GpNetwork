#pragma once

#include "../GpNetwork_global.hpp"

namespace GPlatform {

GP_ENUM(GPNETWORK_API, GpHttpRequestType,
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

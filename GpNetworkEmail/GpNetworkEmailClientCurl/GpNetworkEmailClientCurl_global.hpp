#pragma once

#include "../GpNetworkEmailCore/GpNetworkEmailCore.hpp"

#if defined(GP_NETWORK_EMAIL_CLIENT_CURL_LIBRARY)
    #define GP_NETWORK_EMAIL_CLIENT_CURL_API GP_DECL_EXPORT
#else
    #define GP_NETWORK_EMAIL_CLIENT_CURL_API GP_DECL_IMPORT
#endif

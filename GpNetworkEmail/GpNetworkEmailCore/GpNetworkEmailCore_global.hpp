#pragma once

#include "../../GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCore.hpp"

#if defined(GP_NETWORK_EMAIL_CORE_LIBRARY)
    #define GP_NETWORK_EMAIL_CORE_API GP_DECL_EXPORT
#else
    #define GP_NETWORK_EMAIL_CORE_API GP_DECL_IMPORT
#endif

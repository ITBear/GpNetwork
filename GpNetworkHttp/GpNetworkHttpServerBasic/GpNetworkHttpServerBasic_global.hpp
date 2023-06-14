#pragma once

#include "../../../GpCore2/GpUtils/Macro/GpMacroImportExport.hpp"

#if defined(GP_NETWORK_HTTP_SERVER_BASIC_LIBRARY)
    #define GP_NETWORK_HTTP_SERVER_BASIC_API GP_DECL_EXPORT
#else
    #define GP_NETWORK_HTTP_SERVER_BASIC_API GP_DECL_IMPORT
#endif

#pragma once

#include "../GpCore2/GpCore.hpp"
#include "../GpLog/GpLog.hpp"

#if defined(GPNETWORK_LIBRARY)
    #define GPNETWORK_API GP_DECL_EXPORT
#else
    #define GPNETWORK_API GP_DECL_IMPORT
#endif

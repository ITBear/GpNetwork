#pragma once

#include "../../GpCore2/GpUtils/GpUtils.hpp"
#include "../../GpCore2/GpReflection/GpReflection.hpp"
#include "../../GpCore2/GpTasks/GpTasks.hpp"
#include "../../GpLog/GpLogCore/GpLogCore.hpp"

#if defined(GP_NETWORK_CORE_LIBRARY)
    #define GP_NETWORK_CORE_API GP_DECL_EXPORT
#else
    #define GP_NETWORK_CORE_API GP_DECL_IMPORT
#endif

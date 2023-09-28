#pragma once

#include "GpNetworkCore_global.hpp"
#include "../../GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp"

namespace GPlatform {

#if defined(GP_POSIX)
    using GpIOObjectId  = int;
    constexpr GpIOObjectId GpIOObjectId_Default (void) {return GpIOObjectId(-1);}
#elif defined(GP_OS_WINDOWS)
    using GpIOObjectId  = SOCKET;
    constexpr GpIOObjectId GpIOObjectId_Default (void) {return GpIOObjectId(INVALID_SOCKET);}
#endif

}//namespace GPlatform

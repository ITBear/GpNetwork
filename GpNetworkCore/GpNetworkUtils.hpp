#pragma once

#include "GpNetworkCore_global.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpNetworkUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpNetworkUtils)

public:
    static void     SInitGlobal         (void);
    static void     SFinishGlobal       (void); 
};

}// namespace GPlatform

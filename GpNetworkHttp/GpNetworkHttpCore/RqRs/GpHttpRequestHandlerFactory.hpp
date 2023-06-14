#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../../GpCore2/GpUtils/Macro/GpMacroClass.hpp"
#include "../../../../GpCore2/GpUtils/Types/Containers/GpContainersT.hpp"

namespace GPlatform {

class GpHttpRequestHandler;

class GP_NETWORK_HTTP_CORE_API GpHttpRequestHandlerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestHandlerFactory)
    CLASS_DD(GpHttpRequestHandlerFactory)

protected:
                                        GpHttpRequestHandlerFactory     (void) noexcept {}

public:
    virtual                             ~GpHttpRequestHandlerFactory    (void) noexcept {}

    virtual GpSP<GpHttpRequestHandler>  NewInstance                     (void) const = 0;
};

}//namespace GPlatform

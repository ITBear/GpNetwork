#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "../../../../GpCore2/GpUtils/Macro/GpMacroClass.hpp"
#include "../../../../GpCore2/GpUtils/Types/Containers/GpContainersT.hpp"
#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpRequestHandlerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpRequestHandlerFactory)
    CLASS_DD(GpHttpRequestHandlerFactory)
    TAG_SET(THREAD_SAFE)

protected:
                                        GpHttpRequestHandlerFactory     (void) noexcept = default;

public:
    virtual                             ~GpHttpRequestHandlerFactory    (void) noexcept = default;

    virtual GpHttpRequestHandler::SP    NewInstance                     (void) const = 0;
};

}//namespace GPlatform

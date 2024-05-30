#pragma once

#include "../RqRs/GpHttpRequest.hpp"
#include "../RqRs/GpHttpResponse.hpp"

#include <GpNetwork/GpNetworkCore/Tasks/GpTcpClientTask.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpClient: public GpTcpClientTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpClient)
    CLASS_DD(GpHttpClient)

    enum class ErorrMode
    {
        THROW_ON_NOT_200,
        RETURN_RS
    };

public:
                                GpHttpClient        (GpSocketFlags      aSocketFlags,
                                                     GpIOEventPollerIdx aIOEventPollerIdx,
                                                     milliseconds_t     aConnectTimeout);
                                GpHttpClient        (GpSocketFlags      aSocketFlags,
                                                     GpIOEventPollerIdx aIOEventPollerIdx,
                                                     milliseconds_t     aConnectTimeout,
                                                     std::string        aTaskName);
    virtual                     ~GpHttpClient       (void) noexcept;

    virtual GpHttpResponse::SP  DoRqAndWaitForRs    (GpHttpRequest::SP  aRequestSP,
                                                     ErorrMode          aErorrMode) = 0;
};

}// namespace GPlatform

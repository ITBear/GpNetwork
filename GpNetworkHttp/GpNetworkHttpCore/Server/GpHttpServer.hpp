#pragma once

#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Routers/GpHttpRouter.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Server/GpHttpServerCfgDesc.hpp>
#include <GpNetwork/GpNetworkCore/Tasks/GpTcpAcceptServerTask.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServer
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpServer)
    CLASS_DD(GpHttpServer)
    TAG_SET(THREAD_SAFE)

public:
                                GpHttpServer            (void) noexcept;
                                GpHttpServer            (GpHttpServerCfgDesc    aServerCfgDesc,
                                                         GpHttpRouter::SP       aRouter);
                                ~GpHttpServer           (void) noexcept;

    void                        Start                   (void);
    void                        Start                   (GpHttpServerCfgDesc    aServerCfgDesc,
                                                         GpHttpRouter::SP       aRouter);
    void                        RequestAndWaitForStop   (void);

private:
    mutable GpSpinLock          iSpinLock;
    GpHttpServerCfgDesc         iServerCfgDesc      GUARDED_BY(iSpinLock);
    GpHttpRouter::SP            iRouter             GUARDED_BY(iSpinLock);
    GpTcpAcceptServerTask::SP   iAcceptSocketTask   GUARDED_BY(iSpinLock);
};

}// namespace GPlatform

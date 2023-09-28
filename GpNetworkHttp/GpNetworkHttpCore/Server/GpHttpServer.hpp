#pragma once

#include "../Routers/GpHttpRouter.hpp"
#include "GpHttpServerCfgDesc.hpp"
#include "../../../GpNetworkCore/Tasks/GpTcpAcceptServerTask.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpServer
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpServer)
    CLASS_DD(GpHttpServer)
    TAG_SET(THREAD_SAFE)

public:
                                    GpHttpServer    (void) noexcept;
                                    GpHttpServer    (GpHttpServerCfgDesc    aServerCfgDesc,
                                                     GpHttpRouter::SP       aRouter);
                                    ~GpHttpServer   (void) noexcept;

    void                            Start           (void);
    void                            Start           (GpHttpServerCfgDesc    aServerCfgDesc,
                                                     GpHttpRouter::SP       aRouter);
    void                            RequestStop     (void);
    void                            WaitForStop     (void);

private:
    mutable std::mutex              iMutex;
    GpHttpServerCfgDesc             iServerCfgDesc;
    GpHttpRouter::SP                iRouter;
    GpTcpAcceptServerTask::SP       iAcceptSocketsTask;
};

}//namespace GPlatform

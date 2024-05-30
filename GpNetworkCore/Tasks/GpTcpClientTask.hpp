#pragma once

#include <GpNetwork/GpNetworkCore/Tasks/GpSingleSocketTask.hpp>

namespace GPlatform {

GP_ENUM(GP_NETWORK_CORE_API, GpTcpClientConnectionState,
    NOT_CONNECTED,
    CHECK_CONNECTION,
    CONNECTION_IN_PROGRESS,
    CONNECTED
);

class GP_NETWORK_CORE_API GpTcpClientTask: public GpSingleSocketTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTcpClientTask)
    CLASS_DD(GpTcpClientTask)

public:
                            GpTcpClientTask         (GpSocketFlags      aSocketFlags,
                                                     GpIOEventPollerIdx aIOEventPollerIdx,
                                                     milliseconds_t     aConnectTimeout);
                            GpTcpClientTask         (GpSocketFlags      aSocketFlags,
                                                     GpIOEventPollerIdx aIOEventPollerIdx,
                                                     milliseconds_t     aConnectTimeout,
                                                     std::string        aTaskName);
    virtual                 ~GpTcpClientTask        (void) noexcept override;

    void                    CloseConnection         (void);
    void                    ConnectToAndWait        (const GpSocketAddr& aServerAddr);

protected:
    virtual void            OnReadyToRead           (GpSocket& aSocket) override;
    virtual void            OnReadyToWrite          (GpSocket& aSocket) override;
    virtual void            OnClosed                (GpSocket& aSocket) override;
    virtual void            OnError                 (GpSocket& aSocket) override;
    virtual void            ProcessOtherMessages    (GpAny& aMessage) override;

protected:
    const milliseconds_t    ConnectTimeout          (void) const noexcept {return iConnectTimeout;}

private:
    const milliseconds_t    iConnectTimeout;
};

}// namespace GPlatform

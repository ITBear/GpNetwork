#include <GpNetwork/GpNetworkCore/Tasks/GpTcpClientTask.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCatalog.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketTCP.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpTcpClientConnectionState)

// --------------------------------------------- GpTcpClientTaskFactory --------------------------------------------------

class GpTcpClientSocketFactory final: public GpSocketFactory
{
public:
    CLASS_DD(GpTcpClientSocketFactory)

public:
                            GpTcpClientSocketFactory    (GpSocketFlags aSocketFlags) noexcept;
    virtual                 ~GpTcpClientSocketFactory   (void) noexcept override final;

    virtual GpSocket::SP    NewInstance                 (void) const override final;
    virtual void            OnStart                     (GpSocket::SP aSocket) const override final;
    virtual void            OnStop                      (GpSocket::SP aSocket) const override final;

private:
    const GpSocketFlags     iSocketFlags;
};

GpTcpClientSocketFactory::GpTcpClientSocketFactory (const GpSocketFlags aSocketFlags) noexcept:
iSocketFlags{std::move(aSocketFlags)}
{
}

GpTcpClientSocketFactory::~GpTcpClientSocketFactory (void) noexcept
{
}

GpSocket::SP    GpTcpClientSocketFactory::NewInstance (void) const
{
    // Create socket
    GpSocketTCP::SP socket = MakeSP<GpSocketTCP>
    (
        iSocketFlags | GpSocketFlag::NO_BLOCK,
        GpSocket::CloseModeT::CLOSE_ON_DESTRUCT
    );

    return socket;
}

void    GpTcpClientSocketFactory::OnStart ([[maybe_unused]] GpSocket::SP aSocket) const
{
    // NOP
}

void    GpTcpClientSocketFactory::OnStop ([[maybe_unused]] GpSocket::SP aSocket) const
{
    // NOP
}

// --------------------------------------------- GpTcpClientTask --------------------------------------------------

GpTcpClientTask::GpTcpClientTask
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout
):
GpSingleSocketTask
{
    MakeSP<GpTcpClientSocketFactory>(aSocketFlags),
    aIOEventPollerIdx
},
iConnectTimeout{aConnectTimeout}
{
}

GpTcpClientTask::GpTcpClientTask
(
    const GpSocketFlags         aSocketFlags,
    const GpIOEventPollerIdx    aIOEventPollerIdx,
    const milliseconds_t        aConnectTimeout,
    std::string                 aTaskName
):
GpSingleSocketTask
{
    MakeSP<GpTcpClientSocketFactory>(aSocketFlags),
    aIOEventPollerIdx,
    std::move(aTaskName)
},
iConnectTimeout{aConnectTimeout}
{
}

GpTcpClientTask::~GpTcpClientTask (void) noexcept
{
}

void    GpTcpClientTask::CloseConnection (void)
{
    //?
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpTcpClientTask::ConnectToAndWait ([[maybe_unused]] const GpSocketAddr& aServerAddr)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpTcpClientTask::OnReadyToRead ([[maybe_unused]] GpSocket& aSocket)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpTcpClientTask::OnReadyToWrite ([[maybe_unused]] GpSocket& aSocket)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpTcpClientTask::OnClosed ([[maybe_unused]] GpSocket& aSocket)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpTcpClientTask::OnError ([[maybe_unused]] GpSocket& aSocket)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpTcpClientTask::ProcessOtherMessages (GpAny& aMessage)
{
    THROW_GP
    (
        fmt::format
        (
            "Get not socket message {}",
            aMessage.TypeInfo().name()
        )
    );
}

}// namespace GPlatform

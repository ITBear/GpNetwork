#pragma once

#include <GpNetwork/GpNetworkCore/Sockets/GpSocket.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventType.hpp>

#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpSocketTask: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSocketTask)
    CLASS_DD(GpSocketTask)
    TAG_SET(THREAD_SAFE)

protected:
                                GpSocketTask        (void) noexcept;
                                GpSocketTask        (std::string aTaskName) noexcept;

public:
    virtual                     ~GpSocketTask       (void) noexcept override;

    static constexpr u_int_64   STypeUID            (void) noexcept {return 0xFA062B498C61F555ULL;}
    virtual u_int_64            TypeUID             (void) const noexcept override final;

    // NOTE: After calling PushSocketEvents, WAKEUP_TASK() must be called
    virtual void                PushSocketEvents    (GpSocketId         aSocketId,
                                                     GpIOEventsTypes    aIoEvents) = 0;

protected:
    virtual void                OnStart             (void) override = 0;
    virtual GpTaskRunRes::EnumT OnStep              (void) override = 0;
    virtual void                OnStop              (ExceptionsT& aStopExceptionsOut) noexcept override = 0;
    virtual void                OnStopException     (const GpException& aException) noexcept override = 0;
};

}// namespace GPlatform

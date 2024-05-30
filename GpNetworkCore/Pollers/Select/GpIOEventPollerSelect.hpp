#pragma once

#include "../GpIOEventPoller.hpp"
#include <GpCore2/Config/IncludeExt/boost_flat_set.hpp>

namespace GPlatform {

class GpIOEventPollerSelect final: public GpIOEventPoller
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpIOEventPollerSelect)
    CLASS_DD(GpIOEventPollerSelect)

    using SocketsSetT = boost::container::small_flat_set<GpSocketId, FD_SETSIZE>;

public:
                                GpIOEventPollerSelect   (std::string aName) noexcept;
    virtual                     ~GpIOEventPollerSelect  (void) noexcept override final;

    void                        Configure               (milliseconds_t aMaxStepTime);

protected:
    virtual void                OnStart                 (void) override final;
    virtual GpTaskRunRes::EnumT OnStep                  (void) override final;
    virtual GpException::C::Opt OnStop                  (void) noexcept override final;

    virtual void                OnAddObject             (GpSocketId aSocketId) REQUIRES(iSpinLock) override final;
    virtual void                OnRemoveObject          (GpSocketId aSocketId) REQUIRES(iSpinLock) override final;

private:
    milliseconds_t              iMaxStepTime        GUARDED_BY(iSpinLock);

    fd_set                      iFdSetMaster        GUARDED_BY(iSpinLock);
    fd_set                      iReadFdSetWorking;
    fd_set                      iWriteFdSetWorking;
    fd_set                      iErrorFdSetWorking;
    int                         iMaxSocketId        GUARDED_BY(iSpinLock) = 0;
    SocketsSetT                 iSockets            GUARDED_BY(iSpinLock);
};

}// namespace GPlatform

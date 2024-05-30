#pragma once

#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPoller.hpp>
#include <GpNetwork/GpNetworkCore/Pollers/GpIOEventPollerCfgDesc.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpDictionary.hpp>

namespace GPlatform {

class GpIOEventPollerIdx_TypeShell;
using GpIOEventPollerIdx = GpTypeShell<size_t, GpIOEventPollerIdx_TypeShell>;

class GP_NETWORK_CORE_API GpIOEventPollerCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPollerCatalog)
    TAG_SET(THREAD_SAFE)

    using CatalogNameToIdxT         = boost::container::small_flat_map<std::string, GpIOEventPollerIdx, 8, std::less<>>;
    using CatalogIdxToPollerT       = boost::container::small_vector<GpIOEventPoller::SP, 8>;

    using CreatePollerFnT           = std::function<GpIOEventPoller::SP(std::string_view aName, const GpIOEventPollerCfgDesc& aCfg)>;
    using CreatePollerFnsCatalogT   = boost::container::small_flat_map<GpUUID, CreatePollerFnT, 8>;

private:
                                    GpIOEventPollerCatalog  (void) noexcept;

public:
                                    ~GpIOEventPollerCatalog (void) noexcept;

    static GpIOEventPollerCatalog&  S                       (void) noexcept {return sInstance;}
    [[nodiscard]] static bool       SAddSubscriptionSafe    (GpSocketId         aSocketId,
                                                             GpTaskId           aSocketTaskId,
                                                             GpIOEventPollerIdx aIoEventPollerIdx);
    [[nodiscard]] static bool       SRemoveSubscriptionSafe (GpSocketId         aSocketId,
                                                             GpTaskId           aSocketTaskId,
                                                             GpIOEventPollerIdx aIoEventPollerIdx);

    GpIOEventPoller::C::Opt::SP     GetByIdxOpt             (GpIOEventPollerIdx aIoEventPollerIdx) noexcept;
    GpIOEventPoller::SP             GetByIdx                (GpIOEventPollerIdx aIoEventPollerIdx);

    GpIOEventPoller::C::Opt::SP     GetByNameOpt            (std::string_view aName) noexcept;
    GpIOEventPoller::SP             GetByName               (std::string_view aName);

    GpIOEventPollerIdx              IdxByName               (std::string_view aName);

    void                            Start                   (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs);
    void                            StopAndClear            (void);

private:
    void                            RegisterPollerType      (const GpUUID&      aUid,
                                                             CreatePollerFnT    aFn) REQUIRES(iSpinLockRW);
    std::string_view                NameByIdx               (GpIOEventPollerIdx aIoEventPollerIdx) noexcept REQUIRES(iSpinLockRW);
    GpIOEventPollerIdx              AddNewName              (std::string aName) REQUIRES(iSpinLockRW);

private:
    mutable GpSpinLockRW            iSpinLockRW;
    CatalogNameToIdxT               iCatalogNameToIdx       GUARDED_BY(iSpinLockRW);
    CatalogIdxToPollerT             iCatalogIdxToPoller     GUARDED_BY(iSpinLockRW);
    CreatePollerFnsCatalogT         iRegisteredPollerTypes  GUARDED_BY(iSpinLockRW);

    static GpIOEventPollerCatalog   sInstance;
};

}// namespace GPlatform

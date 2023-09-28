#pragma once

#include "GpIOEventPoller.hpp"
#include "GpIOEventPollerCfgDesc.hpp"
#include "../../../GpCore2/GpUtils/Types/Containers/GpDictionary.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPollerCatalog)
    TAG_SET(THREAD_SAFE)

    using CatalogT                  = GpDictionary<std::u8string, GpIOEventPoller::SP>;
    using CreatePollerFnT           = std::function<GpIOEventPoller::SP(std::u8string_view aName, const GpIOEventPollerCfgDesc& aCfg)>;
    using CreatePollerFnsCatalogT   = std::map<GpUUID, CreatePollerFnT>;

private:
                                    GpIOEventPollerCatalog  (void) noexcept;

public:
                                    ~GpIOEventPollerCatalog (void) noexcept;

    static GpIOEventPollerCatalog&  S                       (void) noexcept {return sInstance;}
    static void                     SStart                  (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs) {S().Start(aCfgs);}
    static void                     SStopAndClear           (void) {S().StopAndClear();}

    const CatalogT&                 Catalog                 (void) const noexcept {return iCatalog;}
    CatalogT&                       Catalog                 (void) noexcept {return iCatalog;}

    inline CatalogT::ValueOptT      Get                     (std::u8string_view aPollerName) noexcept;

private:
    void                            Start                   (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs);
    void                            StopAndClear            (void);

    void                            RegisterPollerType      (const GpUUID&      aUid,
                                                             CreatePollerFnT    aFn);

private:
    CatalogT                        iCatalog;
    CreatePollerFnsCatalogT         iRegisteredPollerTypes;
    static GpIOEventPollerCatalog   sInstance;
};

GpIOEventPollerCatalog::CatalogT::ValueOptT GpIOEventPollerCatalog::Get (std::u8string_view aPollerName) noexcept
{
    return iCatalog.GetOpt(aPollerName);
}

}//namespace GPlatform

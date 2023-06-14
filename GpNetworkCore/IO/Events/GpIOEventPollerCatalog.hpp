#pragma once

#include "GpIOEventPoller.hpp"
#include "GpIOEventPollerCfgDesc.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPollerCatalog)
    TAG_SET(THREAD_SAFE)

    using CatalogT          = GpDictionary<std::u8string, GpIOEventPoller::SP>;
    using StartItcPromiseT  = typename GpIOEventPoller::StartItcPromiseT;
    using StartItcFutureT   = typename GpIOEventPoller::StartItcFutureT;
    using StartItcResultT   = typename GpIOEventPoller::StartItcResultT;

private:
                                    GpIOEventPollerCatalog  (void) noexcept;

public:
                                    ~GpIOEventPollerCatalog (void) noexcept;

    static GpIOEventPollerCatalog&  S                       (void) {return sInstance;}

    void                            Start                   (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs);
    void                            Clear                   (void);
    GpIOEventPoller::SP             Find                    (std::u8string_view aName);

private:
    void                            Register                (std::u8string          aName,
                                                             GpIOEventPoller::SP    aEventPoller);

private:
    CatalogT                        iCatalog;
    static GpIOEventPollerCatalog   sInstance;
};

}//namespace GPlatform

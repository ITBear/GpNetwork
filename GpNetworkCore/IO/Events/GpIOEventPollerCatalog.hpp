#pragma once

#include "GpIOEventPoller.hpp"
#include "GpIOEventPollerCfgDesc.hpp"

namespace GPlatform {

class GP_NETWORK_CORE_API GpIOEventPollerCatalog
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpIOEventPollerCatalog)
    CLASS_TAG(THREAD_SAFE)

    using CatalogT = GpElementsCatalog<std::string, GpIOEventPoller::SP>;

private:
                                        GpIOEventPollerCatalog      (void) noexcept;

public:
                                        ~GpIOEventPollerCatalog     (void) noexcept;

    static GpIOEventPollerCatalog&      S                           (void) {return sCatalog;}

    void                                Start                       (const GpIOEventPollerCfgDesc::C::MapStr::SP& aCfgs);
    void                                Clear                       (void);
    GpIOEventPoller&                    Find                        (std::string_view aName);

private:
    void                                Register                    (std::string            aName,
                                                                     GpIOEventPoller::SP    aEventPoller);

private:
    CatalogT                            iCatalog;
    static GpIOEventPollerCatalog       sCatalog;
};

}//namespace GPlatform

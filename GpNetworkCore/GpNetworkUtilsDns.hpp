#pragma once

#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Units/Other/unix_ts_t.hpp>
#include <GpCore2/GpTasks/ITC/GpItcCacheMap.hpp>
#include <GpNetwork/GpNetworkCore/Sockets/GpSocketAddr.hpp>

namespace GPlatform {

class GP_NETWORK_CORE_API GpNetworkUtilsDns
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpNetworkUtilsDns)
    TAG_SET(THREAD_SAFE)

    struct ResolveRes
    {
        boost::container::small_vector<GpSocketAddr, 4> addresses;
        unix_ts_s_t                                     timestamp;
        size_t                                          useCount    = 0;
        size_t                                          lastGetId   = 0; // For round robin
    };

//  using CacheT = GpItcCacheMap<boost::container::small_flat_map<std::string, ResolveRes, 32, std::less<>>>;

public:
                                GpNetworkUtilsDns   (void) noexcept;
                                ~GpNetworkUtilsDns  (void) noexcept;

/*  static GpNetworkUtilsDns&   S                   (void) noexcept {return sInstance;}

    GpSocketAddr                Resolve             (std::string_view               aDomainName,
                                                     GpSocketIPv                    aIPv,
                                                     GpSocketAddr::C::Opts::CRef    aCurrentResolvedAddrOptCRef = std::nullopt);*/

    static ResolveRes           SResolveNoCache     (std::string_view   aDomainName,
                                                     GpSocketIPv        aIPv);

private:
//  CacheT                      iCache;

    static GpNetworkUtilsDns    sInstance;
};

}// namespace GPlatform

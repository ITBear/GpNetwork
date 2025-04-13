#include <GpNetwork/GpNetworkCore/GpNetworkUtilsDns.hpp>
#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>
#include <GpNetwork/GpNetworkCore/GpNetworkErrors.hpp>

#if defined(GP_POSIX)
#   include <netdb.h>
#endif// #if defined(GP_POSIX)

namespace GPlatform {

GpNetworkUtilsDns   GpNetworkUtilsDns::sInstance;

GpNetworkUtilsDns::GpNetworkUtilsDns (void) noexcept:
iCache
{
    4096/*max cache size*/,
    4/*shards count*/,
    4/*shard init size*/
}
{
}

GpNetworkUtilsDns::~GpNetworkUtilsDns (void) noexcept
{
}

GpSocketAddr    GpNetworkUtilsDns::Resolve
(
    std::string_view            aDomainName,
    GpSocketIPv                 aIPv,
    GpSocketAddr::C::Opts::CRef aCurrentResolvedAddrOptCRef
)
{
    const CacheT::GenerateFnT generateFn = [aDomainName, aIPv]() -> ResolveRes
    {
        return GpNetworkUtilsDns::SResolveNoCache(aDomainName, aIPv);
    };

    static const CacheT::ValidateFnT sValidateFn = [](const ResolveRes& aResolveRes) -> bool
    {
        // 5 min max cache valid timeout
        const unix_ts_s_t nowTS = GpDateTimeOps::SUnixTS_s();
        if ((nowTS - aResolveRes.timestamp) <= 300_unix_ts_s) [[likely]]
        {
            return true;
        }

        return false;
    };

    static const CacheT::EvictionFnT sEvictionFn = [](auto& aContainer) -> void
    {
        // Try to find one timed out element
        auto    minUseCountIter = std::begin(aContainer);
        size_t  minUseCount     = 0;

        for (auto iter = std::begin(aContainer); iter != std::end(aContainer); iter++)
        {
            const ResolveRes& val = iter->second;

            // Check min use count
            if (val.useCount < minUseCount) [[unlikely]]
            {
                minUseCount     = val.useCount;
                minUseCountIter = iter;
            }

            // Validate
            if (!sValidateFn(val))
            {
                aContainer.erase(iter);
                return;
            }
        }

        aContainer.erase(minUseCountIter);
    };

    static const CacheT::IsUseGeneratedValueFnT sIsUseGeneratedValueFn = []([[maybe_unused]] const auto& aInCacheValue, [[maybe_unused]] const auto& aGeneratedValue)
    {
        return true;
    };

    const CacheT::TransformFnT<GpSocketAddr> transformValueFn = [aCurrentResolvedAddrOptCRef](ResolveRes& aResolveRes) -> GpSocketAddr
    {
        aResolveRes.useCount++;
        aResolveRes.lastGetId++;

        const auto& addresses = aResolveRes.addresses;

        if (aCurrentResolvedAddrOptCRef.has_value())
        {
            // Try to find 'aCurrentResolvedAddrOpt' in aResolveRes.addresses
            const auto iter = std::find
            (
                std::begin(addresses),
                std::end(addresses),
                aCurrentResolvedAddrOptCRef.value().get()
            );

            if (iter != addresses.end())
            {
                return *iter;
            }
        }

        // Select by "round robin" algo
        const size_t addressesCount = std::size(addresses);
        return addresses[aResolveRes.lastGetId % addressesCount];
    };

    // Try to find in cache or generate new
    auto[resolveRes, _] = iCache.FindOrGenerate
    (
        aDomainName,
        generateFn,
        sValidateFn,
        sIsUseGeneratedValueFn,
        sEvictionFn,
        transformValueFn
    );

    return resolveRes;
}

GpNetworkUtilsDns::ResolveRes   GpNetworkUtilsDns::SResolveNoCache
(
    std::string_view    aDomainName,
    GpSocketIPv         aIPv
)
{
    struct addrinfo*    resolvedAddrInfo = nullptr;
    struct addrinfo     addrHints;

    GpRAIIonDestruct onDestruct = [&resolvedAddrInfo]()
    {
        // Free the linked list
        if (resolvedAddrInfo != nullptr)
        {
            freeaddrinfo(resolvedAddrInfo);
            resolvedAddrInfo = nullptr;
        }
    };

    memset(&addrHints, 0, sizeof(addrHints));
    addrHints.ai_family = GpSocketIPv_SSFamily(aIPv);

    // Resolve
    const std::string domainName{aDomainName};
    const int getaddrinfoRes = getaddrinfo
    (
        std::data(domainName),
        nullptr,
        &addrHints,
        &resolvedAddrInfo
    );

    // Check resolve result
    VERIFY
    (
        getaddrinfoRes == 0,
        [getaddrinfoRes, domainName]()
        {
#if defined(GP_POSIX)
            const std::string errorMsg = gai_strerror(getaddrinfoRes);
#elif defined(GP_OS_WINDOWS)
            const std::string errorMsg = GpNetworkErrors::SGetLastError();
#else
#   error Unsupported OS
#endif

            return fmt::format
            (                       
                "Failed to get ip address for domain '{}': {}",
                domainName,
                errorMsg
            );
        }
    );

    // Fill ResolveRes
    ResolveRes resolveRes;
    resolveRes.timestamp    = GpDateTimeOps::SUnixTS_s();
    auto& resolvedAddresses = resolveRes.addresses;

    for (struct addrinfo* addrInfo = resolvedAddrInfo; addrInfo != nullptr; addrInfo = addrInfo->ai_next)
    {
        if (addrInfo->ai_family == AF_INET) // ipv4
        {
            const struct sockaddr_in& addr = *reinterpret_cast<struct sockaddr_in*>(addrInfo->ai_addr);
            resolvedAddresses.emplace_back(addr);
        } else // ipv6
        {
            const struct sockaddr_in6& addr = *reinterpret_cast<struct sockaddr_in6*>(addrInfo->ai_addr);
            resolvedAddresses.emplace_back(addr);
        }
    }

    return resolveRes;
}

}// namespace GPlatform

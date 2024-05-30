#include <GpNetwork/GpNetworkCore/GpNetworkUtilsDns.hpp>
#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>

#if defined(GP_POSIX)
#   include <netdb.h>
#endif// #if defined(GP_POSIX)

namespace GPlatform {

GpNetworkUtilsDns   GpNetworkUtilsDns::sInstance;

GpNetworkUtilsDns::GpNetworkUtilsDns (void) noexcept:
iCache{256/*max cache size*/}
{
}

GpNetworkUtilsDns::~GpNetworkUtilsDns (void) noexcept
{
}

GpSocketAddr    GpNetworkUtilsDns::Resolve
(
    std::string_view            aDomainName,
    GpSocketIPv                 aIPv,
    GpSocketAddr::C::Opt::CRef  aCurrentResolvedAddrOptCRef
)
{
    const auto generateNewValueFn = [aDomainName, aIPv]() -> ResolveRes
    {
        const std::string domainName{aDomainName};// Must be null terminated
        return GpNetworkUtilsDns::SResolveNoCache(domainName, aIPv);
    };

    static const auto validateFn = [](const ResolveRes& aResolveRes) -> bool
    {
        // 5 min max cache valid timeout
        const unix_ts_s_t nowTS = GpDateTimeOps::SUnixTS_s();
        if ((nowTS - aResolveRes.timestamp) <= 300_unix_ts_s) [[likely]]
        {
            return true;
        }

        return false;
    };

    static const auto removeOneTimedOutFn = []([[maybe_unused]] auto& aContainer) -> void
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
            if (!validateFn(val))
            {
                aContainer.erase(iter);
                return;
            }
        }

        aContainer.erase(minUseCountIter);
    };

    static const auto onGetValueFn = [aCurrentResolvedAddrOptCRef](ResolveRes& aResolveRes) -> GpSocketAddr
    {
        aResolveRes.useCount++;
        aResolveRes.lastGetId++;

        const auto& addresses = aResolveRes.addresses;

        if (aCurrentResolvedAddrOptCRef.has_value())
        {
            // Try to find 'aCurrentResolvedAddrOpt' in aResolveRes.addresses

            const auto iter = std::find
            (
                addresses.begin(),
                addresses.end(),
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
    return iCache.GetOrGenerateNew<std::string_view, GpSocketAddr>
    (
        aDomainName,
        onGetValueFn,
        generateNewValueFn,
        validateFn,
        removeOneTimedOutFn
    );
}

GpNetworkUtilsDns::ResolveRes   GpNetworkUtilsDns::SResolveNoCache
(
    const std::string&  aDomainName,
    GpSocketIPv         aIPv
)
{
#if defined(GP_POSIX) // -------------------------------------------- GP_POSIX --------------------------------------------
    struct addrinfo*    resolvedAddrInfo = nullptr;
    struct addrinfo     addrHints;

    GpRAIIonDestruct onDestruct
    (
        [&resolvedAddrInfo]()
        {
            // Free the linked list
            if (resolvedAddrInfo != nullptr)
            {
                freeaddrinfo(resolvedAddrInfo);
                resolvedAddrInfo = nullptr;
            }
        }
    );

    memset(&addrHints, 0, sizeof(addrHints));
    addrHints.ai_family = GpSocketIPv_SSFamily(aIPv);

    // Resolve
    const int getaddrinfoRes = getaddrinfo
    (
        std::data(aDomainName),
        nullptr,
        &addrHints,
        &resolvedAddrInfo
    );

    // Check resolve result
    THROW_COND_GP
    (
        getaddrinfoRes == 0,
        [getaddrinfoRes, aDomainName]()
        {
            return fmt::format
            (
                "Failed to get ip address for domain '{}': {}",
                aDomainName,
                gai_strerror(getaddrinfoRes)
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
        } else// ipv6
        {
            const struct sockaddr_in6& addr = *reinterpret_cast<struct sockaddr_in6*>(addrInfo->ai_addr);
            resolvedAddresses.emplace_back(addr);
        }
    }

    return resolveRes;
#endif// #if defined(GP_POSIX)
}

}// namespace GPlatform

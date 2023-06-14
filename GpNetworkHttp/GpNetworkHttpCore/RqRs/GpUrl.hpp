#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "GpUrlAuthority.hpp"
#include "GpUrlQuery.hpp"
#include "GpUrlReflection.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpUrl
{
public:
    CLASS_DD(GpUrl)

public:
                                GpUrl       (void) noexcept;
                                ~GpUrl      (void) noexcept;

    std::u8string               ToString    (void) const;

private:
    std::u8string               scheme;
    GpUrlAuthority              authority;
    std::u8string               path;
    GpUrlQuery                  query;
    std::u8string               fragment;
    //https://i.stack.imgur.com/UQuRm.png
    //scheme:[//authority]path[?query][#fragment]
    //https://api.binance.com/api/v3/order?symbol=LTCBTC&side=BUY&type=LIMIT&timeInForce=GTC
};

}//namespace GPlatform

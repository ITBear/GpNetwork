#pragma once

#include "../GpNetworkHttpCore_global.hpp"
#include "GpUrlAuthority.hpp"
#include "GpUrlQuery.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpUrl
{
public:
    CLASS_DECLARE_DEFAULTS(GpUrl)

public:
                                GpUrl       (void) noexcept;
                                ~GpUrl      (void) noexcept;

    std::string                 ToString    (void) const;

private:
    std::string                 scheme;
    GpUrlAuthority              authority;
    std::string                 path;
    GpUrlQuery                  query;
    std::string                 fragment;
    //https://i.stack.imgur.com/UQuRm.png
    //scheme:[//authority]path[?query][#fragment]
    //https://api.binance.com/api/v3/order?symbol=LTCBTC&side=BUY&type=LIMIT&timeInForce=GTC
};

}//namespace GPlatform

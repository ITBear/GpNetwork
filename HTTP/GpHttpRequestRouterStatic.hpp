#pragma once

#include "GpHttpRequestRouter.hpp"
#include "GpHttpRequestRouteTable.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpRequestRouterStatic final: public GpHttpRequestRouter
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpRequestRouterStatic)
    CLASS_DECLARE_DEFAULTS(GpHttpRequestRouterStatic)

public:
                                    GpHttpRequestRouterStatic   (GpHttpRequestRouteTable::CSP aRouteTable) noexcept;
    virtual                         ~GpHttpRequestRouterStatic  (void) noexcept override final;

protected:
    virtual GpHttpResponse::SP      OnRequest                   (const GpHttpRequest& aRequest) const override final;

private:
    GpHttpRequestRouteTable::CSP    iRouteTable;
};

}//namespace GPlatform

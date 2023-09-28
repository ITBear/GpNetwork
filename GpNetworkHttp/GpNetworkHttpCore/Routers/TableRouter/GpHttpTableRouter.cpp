#include "GpHttpTableRouter.hpp"

namespace GPlatform {

GpHttpTableRouter::~GpHttpTableRouter (void) noexcept
{
}

GpHttpRequestHandler::SP    GpHttpTableRouter::FindHandler (const GpHttpRequestNoBodyDesc& aHttpRqNoBody)
{
    return iRouteTable->FindHandler(aHttpRqNoBody);
}

}//namespace GPlatform

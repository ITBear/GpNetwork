#include "GpHttpClientCurlFactory.hpp"
#include "GpHttpClientCurl.hpp"

namespace GPlatform {

GpHttpClient::SP    GpHttpClientCurlFactory::NewInstance (void) const
{
    return MakeSP<GpHttpClientCurl>();
}

}//namespace GPlatform

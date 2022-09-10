#include "GpSmtpClientCurlFactory.hpp"
#include "GpSmtpClientCurl.hpp"

namespace GPlatform {

GpSmtpClient::SP    GpSmtpClientCurlFactory::NewInstance (void) const
{
    return MakeSP<GpSmtpClientCurl>();
}

}//namespace GPlatform

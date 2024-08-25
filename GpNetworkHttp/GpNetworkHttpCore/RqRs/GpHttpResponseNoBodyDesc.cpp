#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpResponseNoBodyDesc.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Exceptions/GpHttpException.hpp>

namespace GPlatform {

GpHttpResponseNoBodyDesc::~GpHttpResponseNoBodyDesc (void) noexcept
{
}

void    GpHttpResponseNoBodyDesc::Clear (void)
{
    http_version    = HttpVersionT::HTTP_1_1;
    code            = GpHttpResponseCode::NOT_IMPLEMENTED_501;

    headers.Clear();
}

void    GpHttpResponseNoBodyDesc::SetHttpVersion
(
    const size_t aMajor,
    const size_t aMinor
)
{
    if ((aMajor == 1) && (aMinor == 0))
    {
        http_version = HttpVersionTE::HTTP_1_0;
    } else if ((aMajor == 1) && (aMinor == 1))
    {
        http_version = HttpVersionTE::HTTP_1_1;
    } else
    {
        THROW_HTTP(GpHttpResponseCode::BAD_REQUEST_400, "Unsupported HTTP version"_sv);
    }
}

}// namespace GPlatform

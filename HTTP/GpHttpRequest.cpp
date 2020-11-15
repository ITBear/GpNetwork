#include "GpHttpRequest.hpp"
#include "GpHttpException.hpp"

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpHttpRequest, GP_MODULE_UUID)

GpHttpRequest::GpHttpRequest (void) noexcept
{
}

GpHttpRequest::GpHttpRequest (const GpHttpRequest& aRequest):
http_version(aRequest.http_version),
request_type(aRequest.request_type),
url(aRequest.url),
headers(aRequest.headers),
body(aRequest.body)
{
}

GpHttpRequest::GpHttpRequest (GpHttpRequest&& aRequest) noexcept:
http_version(std::move(aRequest.http_version)),
request_type(std::move(aRequest.request_type)),
url(std::move(aRequest.url)),
headers(std::move(aRequest.headers)),
body(std::move(aRequest.body))
{
}

GpHttpRequest::GpHttpRequest (const HttpVersionTE       aHttpVersion,
                              const HttpRequestTypeTE   aRequestType,
                              std::string_view          aUrl,
                              const GpHttpHeaders&      aHeaders,
                              GpBytesArray&&            aBody):
http_version(aHttpVersion),
request_type(aRequestType),
url(aUrl),
headers(aHeaders),
body(std::move(aBody))
{
}

GpHttpRequest::~GpHttpRequest (void) noexcept
{
}

void    GpHttpRequest::SetHttpVersion (const size_t aMajor, const size_t aMinor)
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

void    GpHttpRequest::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(http_version);
    PROP(request_type);
    PROP(url);
    PROP(headers);
    PROP(body);
}

}//namespace GPlatform

#include "GpHttpResponse.hpp"

namespace GPlatform {

GpHttpResponse::GpHttpResponse (void) noexcept
{
}

GpHttpResponse::GpHttpResponse (const HttpVersionTE     aHttpVersion,
                                const CodeTE            aCode,
                                const ContentTypeTE     aContentType,
                                const CharsetTE         aCharset,
                                const ConnectionFlagTE  aConnectionFlag,
                                const CacheControlTE    aCacheControl,
                                GpBytesArray&&          aBody) noexcept:
iHttpVersion(aHttpVersion),
iCode(aCode),
iContentType(aContentType),
iCharset(aCharset),
iConnectionFlag(aConnectionFlag),
iCacheControl(aCacheControl),
iBody(std::move(aBody))
{
}

GpHttpResponse::GpHttpResponse (const GpHttpException& aHttpEx):
GpHttpResponse(HttpVersionTE::HTTP_1_1,
               aHttpEx.Code(),
               ContentTypeTE::TEXT_PLAIN,
               CharsetTE::UTF_8,
               ConnectionFlagTE::CLOSE,
               CacheControlTE::NO_STORE,
               std::move(GpBytesArrayUtils::SMake(aHttpEx.Message())))
{
}

GpHttpResponse::~GpHttpResponse (void) noexcept
{
}

}//namespace GPlatform

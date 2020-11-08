#pragma once

#include "GpHttpVersion.hpp"
#include "GpHttpResponseCode.hpp"
#include "GpHttpContentType.hpp"
#include "GpHttpCharset.hpp"
#include "GpHttpConnectionFlag.hpp"
#include "GpHttpCacheControl.hpp"
#include "GpHttpException.hpp"

namespace GPlatform {

class GPNETWORK_API GpHttpResponse
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpHttpResponse)
    CLASS_DECLARE_DEFAULTS(GpHttpResponse)

    using HttpVersionT      = GpHttpVersion;
    using HttpVersionTE     = HttpVersionT::EnumT;

    using CodeT             = GpHttpResponseCode;
    using CodeTE            = CodeT::EnumT;

    using ContentTypeT      = GpHttpContentType;
    using ContentTypeTE     = ContentTypeT::EnumT;

    using CharsetT          = GpHttpCharset;
    using CharsetTE         = CharsetT::EnumT;

    using ConnectionFlagT   = GpHttpConnectionFlag;
    using ConnectionFlagTE  = ConnectionFlagT::EnumT;

    using CacheControlT     = GpHttpCacheControl;
    using CacheControlTE    = CacheControlT::EnumT;

public:
                                GpHttpResponse  (void) noexcept;
                                GpHttpResponse  (const HttpVersionTE    aHttpVersion,
                                                 const CodeTE           aCode,
                                                 const ContentTypeTE    aContentType,
                                                 const CharsetTE        aCharset,
                                                 const ConnectionFlagTE aConnectionFlag,
                                                 const CacheControlTE   aCacheControl,
                                                 GpBytesArray&&         aBody) noexcept;
                                GpHttpResponse  (const GpHttpException& aHttpEx);
                                ~GpHttpResponse (void) noexcept;

    HttpVersionTE               HttpVersion     (void) const noexcept {return iHttpVersion;}
    CodeTE                      Code            (void) const noexcept {return iCode;}
    ContentTypeTE               ContentType     (void) const noexcept {return iContentType;}
    CharsetTE                   Charset         (void) const noexcept {return iCharset;}
    ConnectionFlagTE            ConnectionFlag  (void) const noexcept {return iConnectionFlag;}
    CacheControlTE              CacheControl    (void) const noexcept {return iCacheControl;}

    const GpBytesArray&         Body            (void) const noexcept {return iBody;}
    GpBytesArray&               Body            (void) noexcept {return iBody;}

private:
    const HttpVersionTE         iHttpVersion    = HttpVersionT::HTTP_1_1;
    const CodeTE                iCode           = CodeT::INTERNAL_SERVER_ERROR_500;
    const ContentTypeTE         iContentType    = ContentTypeT::TEXT_PLAIN;
    const CharsetTE             iCharset        = CharsetT::NOT_SET;
    const ConnectionFlagTE      iConnectionFlag = ConnectionFlagT::CLOSE;
    const CacheControlTE        iCacheControl   = CacheControlT::NO_STORE;
    GpBytesArray                iBody;
};

}//namespace GPlatform

#pragma once

#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCore_global.hpp>
#include <GpNetwork/GpNetworkHttp/llhttp/llhttp.h>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpRequestNoBodyDesc.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/RqRs/GpHttpResponseNoBodyDesc.hpp>
#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/Body/GpHttpBodyPayload.hpp>

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpParser
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpHttpParser)
    CLASS_DD(GpHttpParser)

    struct GpHttpParserSettings: public llhttp_settings_t
    {
        GpHttpParser* iHttpParser = nullptr;
    };

    enum class Mode
    {
        HTTP_REQUEST,
        HTTP_RESPONSE
    };

public:
                                GpHttpParser                (void) noexcept;
                                ~GpHttpParser               (void) noexcept;

    void                        InitCycle                   (Mode aMode);
    void                        FinishCycle                 (void);
    bool                        ParseNext                   (GpSpanByteR aNextDataBlock);
    GpHttpRequestNoBodyDesc&    RqNoBody                    (void) noexcept {return iRqNoBody;}
    GpHttpResponseNoBodyDesc&   RsNoBody                    (void) noexcept {return iRsNoBody;}
    GpHttpBodyPayload::SP       BodyPayload                 (void) noexcept {return iBodyPayloadSP;}

private:
    static int                  SHttp_OnStatus              (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                         Http_OnStatus               (llhttp_t*          aParser,
                                                             std::string_view   aValue);

    static int                  SHttp_OnURL                 (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                         Http_OnURL                  (std::string_view aValue);

    static int                  SHttp_OnHeaderField         (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                         Http_OnHeaderField          (std::string_view aValue);

    static int                  SHttp_OnHeaderValue         (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                         Http_OnHeaderValue          (std::string_view aValue);

    static int                  SHttp_OnHeadersComplete     (llhttp_t* aHttp);
    int                         Http_OnHeadersComplete      (llhttp_t* aHttp);

    static int                  SHttp_OnBody                (llhttp_t* aHttp, const char* aData, const size_t aLength);
    int                         Http_OnBody                 (GpSpanByteR aValue);

    static int                  SHttp_OnMessageBegin        (llhttp_t* aHttp);
    int                         Http_OnMessageBegin         (llhttp_t* aHttp);

    static int                  SHttp_OnMessageComplete     (llhttp_t* aHttp);
    int                         Http_OnMessageComplete      (llhttp_t* aHttp);

    inline static const GpHttpParserSettings&
                                SCastToHttpSettings         (llhttp_t* aHttp);

private:
    Mode                        iMode   = Mode::HTTP_REQUEST;
    llhttp_t                    iLLHttpParser;
    GpHttpParserSettings        iHttpParserSettings;
    std::string                 iHttpParserCurrentHeaderName;
    GpHttpRequestNoBodyDesc     iRqNoBody;
    GpHttpResponseNoBodyDesc    iRsNoBody;
    GpHttpBodyPayload::SP       iBodyPayloadSP;
    bool                        iIsDone = false;
};

const GpHttpParser::GpHttpParserSettings&   GpHttpParser::SCastToHttpSettings (llhttp_t* aHttp)
{
    return *static_cast<const GpHttpParserSettings*>(aHttp->settings);
}

}// namespace GPlatform

#pragma once

#include "GpHttpResponse.hpp"

namespace GPlatform {

class GpHttpResponseSerializer
{
public:
    CLASS_REMOVE_CTRS(GpHttpResponseSerializer)

public:
    static void                 SSerializeHeaders       (const GpHttpResponse&  aResponse,
                                                         GpByteWriter&          aWriter);

private:
    static void                 SHeadersFirstLine       (const GpHttpResponse&  aResponse,
                                                         GpByteWriter&          aWriter);
    static void                 SHeaders                (const GpHttpResponse&  aResponse,
                                                         GpByteWriter&          aWriter);

private:
    static GpArray<std::string, GpHttpVersion::SCount().As<size_t>()>           sHttpVersion;
    static GpArray<std::string, GpHttpResponseCode::SCount().As<size_t>()>      sHttpResponseCode;
    static GpArray<std::string, GpHttpContentType::SCount().As<size_t>()>       sHttpContentType;
    static GpArray<std::string, GpHttpCharset::SCount().As<size_t>()>           sHttpCharset;
    static GpArray<std::string, GpHttpConnectionFlag::SCount().As<size_t>()>    sHttpConnectionFlag;
    static GpArray<std::string, GpHttpCacheControl::SCount().As<size_t>()>      sHttpCacheControl;
};

}//namespace GPlatform

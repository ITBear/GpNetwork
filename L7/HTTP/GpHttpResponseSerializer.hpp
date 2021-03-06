#pragma once

#include "GpHttpResponse.hpp"

namespace GPlatform {

class GpHttpResponseSerializer
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpHttpResponseSerializer)

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
};

}//namespace GPlatform

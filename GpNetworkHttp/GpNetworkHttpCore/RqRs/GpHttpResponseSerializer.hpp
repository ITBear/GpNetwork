#pragma once

#include "GpHttpResponse.hpp"

namespace GPlatform {

class GP_NETWORK_HTTP_CORE_API GpHttpResponseSerializer
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
    static std::array<std::string, GpHttpVersion::SCount()>         sHttpVersion;
    static std::array<std::string, GpHttpResponseCode::SCount()>    sHttpResponseCode;
};

}//namespace GPlatform

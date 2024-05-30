#include "GpHttpResponseCode.hpp"

#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>

namespace GPlatform {

GP_ENUM_IMPL(GpHttpResponseCode)

static boost::container::small_flat_map<GpHttpResponseCode::EnumT, std::string, GpHttpResponseCode::SCount()> sSGpHttpResponseCode_ToStrMap =
{
    {GpHttpResponseCode::CONTINUE_100                       , "100 Continue"},
    {GpHttpResponseCode::SWITCHING_PROTOCOLS_101            , "101 Switching Protocols"},
    {GpHttpResponseCode::PROCESSING_102                     , "102 Processing"},
    {GpHttpResponseCode::EARLY_HINTS_103                    , "103 Early Hints"},
    {GpHttpResponseCode::OK_200                             , "200 OK"},
    {GpHttpResponseCode::CREATED_201                        , "201 Created"},
    {GpHttpResponseCode::ACCEPTED_202                       , "202 Accepted_"},
    {GpHttpResponseCode::NON_AUTHORITATIVE_INFORMATION_203  , "203 Non Authoritative Information"},
    {GpHttpResponseCode::NO_CONTENT_204                     , "204 No Content"},
    {GpHttpResponseCode::RESET_CONTENT_205                  , "205 Reset Content"},
    {GpHttpResponseCode::PARTIAL_CONTENT_206                , "206 Partial Content"},
    {GpHttpResponseCode::MULTIPLE_CHOICES_300               , "300 Multiple Choices"},
    {GpHttpResponseCode::MOVED_PERMANENTLY_301              , "301 Moved Permanently"},
    {GpHttpResponseCode::FOUND_302                          , "302 Found"},
    {GpHttpResponseCode::SEE_OTHER_303                      , "303 See Other"},
    {GpHttpResponseCode::NOT_MODIFIED_304                   , "304 Not Modified"},
    {GpHttpResponseCode::USE_PROXY_305                      , "305 Use Proxy"},
    {GpHttpResponseCode::SWITCH_PROXY_306                   , "306 Switch Proxy"},
    {GpHttpResponseCode::TEMPORARY_REDIRECT_307             , "307 Temporary Redirect"},
    {GpHttpResponseCode::PERMANENT_REDIRECT_308             , "308 Permanent Redirect"},
    {GpHttpResponseCode::BAD_REQUEST_400                    , "400 Bad Request"},
    {GpHttpResponseCode::UNAUTHORIZED_401                   , "401 Unauthorized"},
    {GpHttpResponseCode::PAYMENT_REQUIRED_402               , "402 Payment Required"},
    {GpHttpResponseCode::FORBIDDEN_403                      , "403 Forbidden"},
    {GpHttpResponseCode::NOT_FOUND_404                      , "404 Not Found"},
    {GpHttpResponseCode::METHOD_NOT_ALLOWED_405             , "405 Method Not Allowed"},
    {GpHttpResponseCode::NOT_ACCEPTABLE_406                 , "406 Not Acceptable"},
    {GpHttpResponseCode::PROXY_AUTHENTICATION_REQUIRED_407  , "407 Proxy Authentication Required"},
    {GpHttpResponseCode::REQUEST_TIMEOUT_408                , "408 Request Timeout"},
    {GpHttpResponseCode::CONFLICT_409                       , "409 Conflict"},
    {GpHttpResponseCode::GONE_410                           , "410 Gone"},
    {GpHttpResponseCode::LENGTH_REQUIRED_411                , "411 Length Required"},
    {GpHttpResponseCode::PRECONDITION_FAILED_412            , "412 Precondition Failed"},
    {GpHttpResponseCode::PAYLOAD_TOO_LARGE_413              , "413 Payload Too Large"},
    {GpHttpResponseCode::URI_TOO_LONG_414                   , "414 Uri Too Long"},
    {GpHttpResponseCode::UNSUPPORTED_MEDIA_TYPE_415         , "415 Unsupported Media Type"},
    {GpHttpResponseCode::RANGE_NOT_SATISFIABLE_416          , "416 Range Not Satisfiable"},
    {GpHttpResponseCode::EXPECTATION_FAILED_417             , "417 Expectation Failed"},
    {GpHttpResponseCode::INTERNAL_SERVER_ERROR_500          , "500 Internal Server Error"},
    {GpHttpResponseCode::NOT_IMPLEMENTED_501                , "501 Not Implemented"},
    {GpHttpResponseCode::BAD_GATEWAY_502                    , "502 Bad Gateway"},
    {GpHttpResponseCode::SERVICE_UNAVAILABLE_503            , "503 Service Unavailable"},
    {GpHttpResponseCode::GATEWAY_TIMEOUT_504                , "504 Gateway Timeout"},
    {GpHttpResponseCode::HTTP_VERSION_NOT_SUPPORTED_505     , "505 Http Version Not Supported"},
    {GpHttpResponseCode::VARIANT_ALSO_NEGOTIATES_506        , "506 Variant Also Negotiates"}
};

std::string_view SGpHttpResponseCode_ToStr (GpHttpResponseCode::EnumT aCode)
{
    const auto iter = sSGpHttpResponseCode_ToStrMap.find(aCode);

    if (iter != std::end(sSGpHttpResponseCode_ToStrMap))
    {
        return iter->second;
    } else
    {
        return {};
    }
}

}// namespace GPlatform

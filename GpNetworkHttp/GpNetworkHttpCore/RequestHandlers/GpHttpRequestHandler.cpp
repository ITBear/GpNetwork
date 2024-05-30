#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

GpHttpResponse::SP  GpHttpRequestHandler::ProcessRequest (GpHttpRequest::SP aRequest)
{
    GpHttpResponse::SP res;

    try
    {
        res = OnRequest(aRequest.V());
    } catch (const GpHttpException& e)
    {
        res = GpHttpResponse::SFromException(e, aRequest->iRequestNoBody.http_version);
    } catch (const std::exception& e)
    {
        res = GpHttpResponse::SFromException(e, aRequest->iRequestNoBody.http_version);
    }

    return res;
}

}// namespace GPlatform

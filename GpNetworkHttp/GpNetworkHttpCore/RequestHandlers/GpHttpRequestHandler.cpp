#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

GpHttpResponse::SP  GpHttpRequestHandler::ProcessRequest (GpHttpRequest::SP aRequest)
{
    GpHttpResponse::SP res;

    try
    {
        res = OnRequest(aRequest);
    } catch (const GpHttpException& e)
    {
        res = GpHttpResponse::SFromException(e, aRequest->iRequestNoBody);
    } catch (const std::exception& e)
    {
        res = GpHttpResponse::SFromException(e, aRequest->iRequestNoBody);
    }

    return res;
}

}//namespace GPlatform

#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

GpHttpResponse::SP  GpHttpRequestHandler::ProcessRequest (GpHttpRequest& aRequest)
{
    GpHttpResponse::SP res;

    try
    {
        res = OnRequest(aRequest);
    } catch (const GpHttpException& e)
    {
        res = MakeSP<GpHttpResponse>();
        res.V().SetFromException(e);
    } catch (const std::exception& e)
    {
        res = MakeSP<GpHttpResponse>();
        res.V().SetFromException(e);
    }

    return res;
}

}//namespace GPlatform

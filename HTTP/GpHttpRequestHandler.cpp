#include "GpHttpRequestHandler.hpp"

namespace GPlatform {

GpHttpResponse::SP  GpHttpRequestHandler::ProcessRequest (const GpHttpRequest& aRequest) const
{
    GpHttpResponse::SP res;

    try
    {
        res = OnRequest(aRequest);
    } catch (const GpHttpException& e)
    {
        res = MakeSP<GpHttpResponse>();
        res->SetFromException(e);
    } catch (const std::exception& e)
    {
        res = MakeSP<GpHttpResponse>();
        res->SetFromException(e);
    }

    return res;
}

}//namespace GPlatform

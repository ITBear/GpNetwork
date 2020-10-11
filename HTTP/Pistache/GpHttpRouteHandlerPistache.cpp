#include "GpHttpRouteHandlerPistache.hpp"
#include "../GpHttpRequestHandler.hpp"

namespace GPlatform {

GpHttpRouteHandlerPistache::GpHttpRouteHandlerPistache (Pistache::Rest::Router&         aHttpRouter,
                                                        GpHttpRequestHandlerFactory::SP aRequestHandlerFactory):
iRequestHandlerFactory(std::move(aRequestHandlerFactory))
{
    // '/'
    Pistache::Rest::Routes::Get(aHttpRouter, "/", Pistache::Rest::Routes::bind(&GpHttpRouteHandlerPistache::Handle, this));
    Pistache::Rest::Routes::Post(aHttpRouter, "/", Pistache::Rest::Routes::bind(&GpHttpRouteHandlerPistache::Handle, this));
}

GpHttpRouteHandlerPistache::~GpHttpRouteHandlerPistache (void) noexcept
{
}

void    GpHttpRouteHandlerPistache::Handle (const Pistache::Rest::Request&  aRequest,
                                            Pistache::Http::ResponseWriter  aResponseWriter)
{
    Pistache::Http::Code    httpResCode = Pistache::Http::Code::Ok;
    std::string             httpResData;

    try
    {
        GpHttpRequestHandler::SP    requestHandler  = iRequestHandlerFactory.VC().NewInstance();
        GpBytesArray                requestBody     = GpBytesArrayUtils::SMake(aRequest.body());
        GpHttpRequest::SP           request         = MakeSP<GpHttpRequest>(""_sv,
                                                                            ""_sv,
                                                                            std::move(requestBody));
        GpHttpResponse::SP          responce        = requestHandler.V().OnRequest(request);
        const GpBytesArray&         responceBody    = responce.VC().Body();

        httpResData = std::string(reinterpret_cast<const char*>(responceBody.data()), responceBody.size());

        //iResponseWriter.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        //iResponseWriter.headers().add<Pistache::Http::Header::Connection>(Pistache::Http::ConnectionControl::Close);

    } catch (const GpException& e)
    {
        httpResCode = Pistache::Http::Code::Internal_Server_Error;
        httpResData = std::string(e.what());
    } catch (const std::exception& e)
    {
        httpResCode = Pistache::Http::Code::Internal_Server_Error;
        httpResData = std::string(e.what());
    } catch (...)
    {
        httpResCode = Pistache::Http::Code::Internal_Server_Error;
        httpResData.clear();
    }

    aResponseWriter.send(httpResCode, httpResData);
}

}//namespace GPlatform

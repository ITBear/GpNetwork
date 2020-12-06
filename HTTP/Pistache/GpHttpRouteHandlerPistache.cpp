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
    microseconds_t beginTS = GpDateTimeOps::SHighResTS_us();

    Pistache::Http::Code    httpResCode = Pistache::Http::Code::Ok;
    std::string             httpErrorData;
    GpHttpResponse::SP      responce;

    try
    {
        GpHttpRequestHandler::SP    requestHandler  = iRequestHandlerFactory->NewInstance();
        GpBytesArray                requestBody     = GpBytesArrayUtils::SMake(aRequest.body());

        std::cout << "@@@[GpHttpRouteHandlerPistache::Handle]: --------------------- RQ ---------------------\n "
                  << GpRawPtrCharR(requestBody).AsStringView() << std::endl;

        GpHttpRequest::SP request = MakeSP<GpHttpRequest>(GpHttpVersion::HTTP_1_1,
                                                          GpHttpRequestType::GET,
                                                          ""_sv,
                                                          GpHttpHeaders(),
                                                          std::move(requestBody));

        responce = requestHandler->ProcessRequest(request.VC());

        //iResponseWriter.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        //iResponseWriter.headers().add<Pistache::Http::Header::Connection>(Pistache::Http::ConnectionControl::Close);

    } catch (const GpException& e)
    {
        httpResCode     = Pistache::Http::Code::Internal_Server_Error;
        httpErrorData   = std::string(e.what());
    } catch (const std::exception& e)
    {
        httpResCode     = Pistache::Http::Code::Internal_Server_Error;
        httpErrorData   = std::string(e.what());
    } catch (...)
    {
        httpResCode = Pistache::Http::Code::Internal_Server_Error;
        httpErrorData.clear();
    }

    if (httpErrorData.length() > 0)
    {
        aResponseWriter.send(httpResCode, httpErrorData);
    } else
    {
        GpRawPtrCharR responceBody(responce->body);

        std::cout << "@@@[GpHttpRouteHandlerPistache::Handle]: RS:\n "
                  << responceBody.AsStringView() << std::endl;

        aResponseWriter.send(httpResCode, responceBody.Ptr(), responceBody.SizeLeft().As<size_t>());
    }

    microseconds_t endTS = GpDateTimeOps::SHighResTS_us();
    std::cout << "@@@[GpHttpRouteHandlerPistache::Handle]: Total time: " << (endTS-beginTS).Value() << "us" << std::endl;
}

}//namespace GPlatform

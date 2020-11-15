#include "GpHttpServerPistache.hpp"

namespace GPlatform {

GpHttpServerPistache::GpHttpServerPistache (GpHttpRequestHandlerFactory::SP aRequestHandlerFactory) noexcept:
GpHttpServer(std::move(aRequestHandlerFactory))
{
}

GpHttpServerPistache::~GpHttpServerPistache (void) noexcept
{
}

void    GpHttpServerPistache::OnStart (void)
{
    Pistache::Address   addr(Pistache::Ipv4::any(), Pistache::Port(8080));
    auto                opts = Pistache::Http::Endpoint::options()
                               .threads(1)
                               .flags(Pistache::Tcp::Options::ReuseAddr |
                                      Pistache::Tcp::Options::ReusePort |
                                      Pistache::Tcp::Options::NoDelay   |
                                      Pistache::Tcp::Options::Linger)
                               .maxRequestSize(8192*2)
                               .maxResponseSize(1024*512);

    iHttpRouteHandler = MakeSP<GpHttpRouteHandlerPistache>(iHttpRouter,
                                                           RequestHandlerFactory());

    iHttpServer = std::make_shared<Pistache::Http::Endpoint>(addr);
    iHttpServer->init(opts);
    iHttpServer->setHandler(iHttpRouter.handler());
    iHttpServer->serveThreaded();

    std::cout << "[NetworkTest::StartHttpServer]: Wait for request..."_sv << std::endl;
}

GpTask::ResT    GpHttpServerPistache::OnStep (EventOptRefT /*aEvent*/)
{
    return GpTask::ResT::WAITING;
}

void    GpHttpServerPistache::OnStop (void) noexcept
{
    std::cout << "[NetworkTest::StopHttpServer]: Shutdown server..."_sv << std::endl;
    iHttpServer->shutdown();
    iHttpServer.reset();
}

}//namespace GPlatform

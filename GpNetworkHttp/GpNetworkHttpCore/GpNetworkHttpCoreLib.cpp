#include <GpNetwork/GpNetworkHttp/GpNetworkHttpCore/GpNetworkHttpCoreLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpNetworkHttpCore)
GP_LIB_REGISTRATOR(GpNetworkHttpCoreLib)

void    GpNetworkHttpCore_StaticInitializer::OnInitialize (void)
{
    GpNetworkHttpCoreLib::SRegisterSelf();
}

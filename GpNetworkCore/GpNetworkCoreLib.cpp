#include <GpNetwork/GpNetworkCore/GpNetworkCoreLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpNetworkCore)
GP_LIB_REGISTRATOR(GpNetworkCoreLib)

void    GpNetworkCore_StaticInitializer::OnInitialize (void)
{
    GpNetworkCoreLib::SRegisterSelf();
}

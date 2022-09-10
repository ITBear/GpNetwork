TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=17cab5e2-7e0b-4d44-8b69-deb8c902ffd8
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_NETWORK_HTTP_CORE_LIBRARY
PACKET_NAME     = GpNetworkHttpCore
DIR_LEVEL       = ./../../../

include(../../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
    Client/GpHttpClientGuard.cpp \
    Client/GpHttpClientPool.cpp \
    Client/GpHttpClientPoolCatalog.cpp \
    Exceptions/GpHttpException.cpp \
    Headers/GpHttpCacheControl.cpp \
    Headers/GpHttpCharset.cpp \
    Headers/GpHttpConnectionFlag.cpp \
    Headers/GpHttpContentType.cpp \
    Headers/GpHttpHeaderType.cpp \
    Headers/GpHttpHeaders.cpp \
    Headers/GpHttpProtoHeaders.cpp \
    Headers/GpHttpRequestType.cpp \
    Headers/GpHttpResponseCode.cpp \
    Headers/GpHttpVersion.cpp \
    Headers/GpProtoHeaderValue.cpp \
    RqRs/GpHttpRequest.cpp \
    RqRs/GpHttpRequestDoneEvent.cpp \
    RqRs/GpHttpRequestHandler.cpp \
    RqRs/GpHttpRequestRouteTable.cpp \
    RqRs/GpHttpRequestRouterStatic.cpp \
    RqRs/GpHttpRequestTask.cpp \
    RqRs/GpHttpResponse.cpp \
    RqRs/GpHttpResponseSerializer.cpp \
    RqRs/GpHttpRqRs.cpp \
    RqRs/GpUrl.cpp \
    RqRs/GpUrlAuthority.cpp \
    RqRs/GpUrlQuery.cpp \
    Server/GpHttpServer.cpp \
    Server/GpHttpServerCfgDesc.cpp

HEADERS += \
    Client/GpHttpClient.hpp \
    Client/GpHttpClientFactory.hpp \
    Client/GpHttpClientGuard.hpp \
    Client/GpHttpClientPool.hpp \
    Client/GpHttpClientPoolCatalog.hpp \
    Exceptions/GpHttpException.hpp \
    GpNetworkHttpCore.hpp \
    GpNetworkHttpCore_global.hpp \
    Headers/GpHttpCacheControl.hpp \
    Headers/GpHttpCharset.hpp \
    Headers/GpHttpConnectionFlag.hpp \
    Headers/GpHttpContentType.hpp \
    Headers/GpHttpHeaderType.hpp \
    Headers/GpHttpHeaders.hpp \
    Headers/GpHttpProtoHeaders.hpp \
    Headers/GpHttpRequestType.hpp \
    Headers/GpHttpResponseCode.hpp \
    Headers/GpHttpVersion.hpp \
    Headers/GpProtoHeaderValue.hpp \
    RqRs/GpHttpRequest.hpp \
    RqRs/GpHttpRequestDoneEvent.hpp \
    RqRs/GpHttpRequestHandler.hpp \
    RqRs/GpHttpRequestHandlerFactory.hpp \
    RqRs/GpHttpRequestRouteTable.hpp \
    RqRs/GpHttpRequestRouter.hpp \
    RqRs/GpHttpRequestRouterStatic.hpp \
    RqRs/GpHttpRequestTask.hpp \
    RqRs/GpHttpResponse.hpp \
    RqRs/GpHttpResponseSerializer.hpp \
    RqRs/GpHttpRqRs.hpp \
    RqRs/GpUrl.hpp \
    RqRs/GpUrlAuthority.hpp \
    RqRs/GpUrlQuery.hpp \
    Server/GpHttpServer.hpp \
    Server/GpHttpServerCfgDesc.hpp \
    Server/GpHttpServerFactory.hpp \
    Server/GpHttpServerSocketTaskFactory.hpp

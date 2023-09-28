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
    Body/GpHttpBodyPayload.cpp \
    Body/GpHttpBodyPayloadFixed.cpp \
    Body/GpHttpBodyPayloadType.cpp \
    Enums/GpHttpCacheControl.cpp \
    Enums/GpHttpCharset.cpp \
    Enums/GpHttpConnectionFlag.cpp \
    Enums/GpHttpContentType.cpp \
    Enums/GpHttpHeaderType.cpp \
    Enums/GpHttpRequestType.cpp \
    Enums/GpHttpResponseCode.cpp \
    Enums/GpHttpVersion.cpp \
    Exceptions/GpHttpException.cpp \
    Headers/GpHttpHeaders.cpp \
    Headers/GpProtoHeaderValue.cpp \
    Headers/GpProtoHeadersMap.cpp \
    RequestHandlers/GpHttpRequestHandler.cpp \
    Routers/RulesRouter/GpHttpRouterRule.cpp \
    Routers/TableRouter/GpHttpRouteTableStatic.cpp \
    Routers/TableRouter/GpHttpTableRouter.cpp \
    RqRs/GpHttpRequest.cpp \
    RqRs/GpHttpRequestNoBodyDesc.cpp \
    RqRs/GpHttpResponse.cpp \
    RqRs/GpHttpResponseNoBodyDesc.cpp \
    Server/GpHttpRequestSocketTask.cpp \
    Server/GpHttpRequestSocketTaskFactory.cpp \
    Server/GpHttpResponseSerializer.cpp \
    Server/GpHttpServer.cpp \
    Server/GpHttpServerCfgDesc.cpp \
    Url/GpUrl.cpp \
    Url/GpUrlAuthority.cpp \
    Url/GpUrlQuery.cpp \
    Url/GpUrlReflection.cpp


HEADERS += \
    Body/GpHttpBodyPayload.hpp \
    Body/GpHttpBodyPayloadFixed.hpp \
    Body/GpHttpBodyPayloadType.hpp \
    Enums/GpHttpCacheControl.hpp \
    Enums/GpHttpCharset.hpp \
    Enums/GpHttpConnectionFlag.hpp \
    Enums/GpHttpContentType.hpp \
    Enums/GpHttpHeaderType.hpp \
    Enums/GpHttpRequestType.hpp \
    Enums/GpHttpResponseCode.hpp \
    Enums/GpHttpVersion.hpp \
    Exceptions/GpHttpException.hpp \
    GpNetworkHttpCore_global.hpp \
    Headers/GpHttpHeaders.hpp \
    Headers/GpProtoHeaderValue.hpp \
    Headers/GpProtoHeadersMap.hpp \
    RequestHandlers/GpHttpRequestHandler.hpp \
    RequestHandlers/GpHttpRequestHandlerFactory.hpp \
    Routers/GpHttpRouter.hpp \
    Routers/RulesRouter/GpHttpRouterRule.hpp \
    Routers/TableRouter/GpHttpRouteTable.hpp \
    Routers/TableRouter/GpHttpRouteTableStatic.hpp \
    Routers/TableRouter/GpHttpTableRouter.hpp \
    RqRs/GpHttpRequest.hpp \
    RqRs/GpHttpRequestNoBodyDesc.hpp \
    RqRs/GpHttpResponse.hpp \
    RqRs/GpHttpResponseNoBodyDesc.hpp \
    Server/GpHttpRequestSocketTask.hpp \
    Server/GpHttpRequestSocketTaskFactory.hpp \
    Server/GpHttpResponseSerializer.hpp \
    Server/GpHttpServer.hpp \
    Server/GpHttpServerCfgDesc.hpp \
    Url/GpUrl.hpp \
    Url/GpUrlAuthority.hpp \
    Url/GpUrlQuery.hpp \
    Url/GpUrlReflection.hpp

